#ifdef _LOGX_IMPLEMENTATION_HPP
#	error "implementation.hpp should one be included once!"
#endif


#include <mutex>
#include <iostream>
#include <thread>
#include <array>
#include "core.hpp"
#include "sink.hpp"
#include "tag.hpp"
#include "details/formatter.hpp"
#include "details/memory.hpp"

namespace logx {

	namespace details {
		template<typename Dummy = void>
		class sink_message_impl : public sink_message
		{
		public:
			sink_message_impl(message_base* message)
				: mMessage(message)
			{
				mArgs = mMessage->args();

				mArgsAsString.reserve(mArgs.size());
				for (auto arg : mArgs)
				{
					mArgsAsString.emplace_back(arg->to_string());
					
					auto* asTag = arg->as_tag();
					if (asTag)
						mTags.emplace_back(asTag);
				}

				mDescription = format(mMessage->msg(), mArgsAsString);
			}

			virtual const string& msg() const override
			{
				return mDescription;
			}

			virtual const std::vector<string>& args() const override
			{
				return mArgsAsString;
			}

			virtual const std::vector<const tag*>& tags() const override
			{
				return mTags;
			}

		private:
			message_base* mMessage;
			string mDescription;
			std::vector<const message_arg*> mArgs;
			std::vector<string> mArgsAsString;
			std::vector<const tag*> mTags;
		};

		template<typename Dummy = void>
		class log_core_impl : public log_core < Dummy >
		{
		public:
			log_core_impl()
				: mRunning(true)
			{
				mSinks.push_back(&log_core_impl::_default_sink);
				_start();
			}

			virtual ~log_core_impl() override
			{
				_stop();
			}


			virtual void init() override
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				mSinks.clear();
			}

			virtual void add_sink(sink _sink) override
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				mSinks.push_back(_sink);
			}

		protected:
			void _start()
			{
#ifndef LOGXCFG_SYNC
				mWorkerThread = std::thread(std::bind(&log_core_impl::_work, this));
#endif
			}

			void _stop()
			{
#ifndef LOGXCFG_SYNC
				mRunning = false;
				mWorkerThread.join();
#endif
			}

			static void _default_sink(const logx::sink_message& msg)
			{
#ifdef LOGXCFG_USE_WCHAR
				std::wcout
#else
				std::cout
#endif
					<< msg.msg() << std::endl;
			}


			void _notify_message(log_thread<>& _thread)
			{
				//while (_thread.mMessages.size())
				//{
				//	auto msg = std::move(_thread.mMessages.front());
				//	_send_sink(msg.get());
				//	_thread.mMessages.pop_front();
				//}
			}

			void _send_sink(message_base* msg)
			{
				sink_message_impl<> sink_msg(msg);

				std::lock_guard<std::mutex> lock(mSinkMutex);
				for (auto& sink : mSinks)
				{
					sink(sink_msg);
				}
			}

#ifdef LOGXCFG_SYNC
			struct _sync_creator : public log_core<>::_msg_creator
			{
				_sync_creator(std::unique_lock<std::mutex>&& _mutex, log_core_impl<>* _core_impl)
					: mutex(std::move(_mutex))
					, core_impl(_core_impl)
				{
				}

				virtual ~_sync_creator() override
				{
					::operator delete(msg_mem);
				}

				virtual void done(message_base* msg) override
				{
					core_impl->_send_sink(msg);
					core_impl->mSyncCreator.destruct();
				}

				log_core_impl* core_impl;
				std::unique_lock<std::mutex> mutex;
			};

			virtual log_core<>::_msg_creator* _get_creator(std::size_t _msg_size) override
			{
				std::unique_lock<std::mutex> lock(mMsgMutex);
				mSyncCreator.construct(std::move(lock), this);

				mSyncCreator->msg_mem = ::operator new(_msg_size);

				return mSyncCreator.get();
			}
#else
#error Not implemented! Please define LOGXCFG_SYNC.

			void _work()
			{
				while (mRunning)
				{

				}

				std::cout << "Exit!" << std::endl;
			}
#endif

		private:
			std::vector<sink> mSinks;

			std::mutex mSinkMutex;
			std::mutex mMsgMutex;

			std::thread mWorkerThread;
			bool mRunning;

#ifdef LOGXCFG_SYNC
			uninitialized<_sync_creator> mSyncCreator;
#endif
		};
	}

	std::shared_ptr<core> core::GLogCore = std::make_shared<details::log_core_impl<>>();

	core& core::get_core()
	{
		return *GLogCore;
	}

	void core::set_core(core& _core)
	{
		GLogCore = _core.shared_from_this();
	}
}
