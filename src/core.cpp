#include <mutex>
#include <iostream>
#include <thread>
#include <array>
#include <unordered_map>
#include <typeindex>
#include "logx/core.hpp"
#include "logx/sink.hpp"
#include "logx/tag.hpp"
#include "logx/text_sink.hpp"
#include <logx/logx_api.hpp>

#include "sink_message.hpp"
#include "memory.hpp"

namespace logx {

	namespace impl {

		class core_impl : public core
		{
		public:
			core_impl()
				: mRunning(true)
			{
#ifdef LOGXCFG_USE_WCHAR
				auto sink = std::make_shared<text_sink>(std::wcout);
#else
				auto sink = std::make_shared<text_sink>(std::cout);
#endif
				mSinks.push_back(std::bind(&text_sink::on_message, sink, std::placeholders::_1));
				_start();
			}

			virtual ~core_impl() override
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

			virtual bool remove_default_tag(const std::type_info& _ty) override
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				return mDefaultTags.erase(_ty) > 0;
			}

		protected:
			virtual std::shared_ptr<tag> _add_default_tag(const std::type_info& _ty, const std::shared_ptr<tag>& _tag) override
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				auto it = mDefaultTags.find(_ty);

				if (it == mDefaultTags.end())
				{
					mDefaultTags.emplace(_ty, _tag);
					return nullptr;
				}
				else{
					std::shared_ptr<tag> result = _tag;
					std::swap(it->second, result);
					return result;
				}
			}

			void _start()
			{
#ifndef LOGXCFG_SYNC
				mWorkerThread = std::thread(std::bind(&core_impl::_work, this));
#endif
			}

			void _stop()
			{
#ifndef LOGXCFG_SYNC
				mRunning = false;
				mWorkerThread.join();
#endif
			}


			void _notify_message(log_thread& _thread)
			{
				//while (_thread.mMessages.size())
				//{
				//	auto msg = std::move(_thread.mMessages.front());
				//	_send_sink(msg.get());
				//	_thread.mMessages.pop_front();
				//}
			}

			void _send_sink(details::message_base* msg)
			{
				sink_message_impl sink_msg(msg, mDefaultTags);

				std::lock_guard<std::mutex> lock(mSinkMutex);
				for (auto& sink : mSinks)
				{
					sink(sink_msg);
				}
			}

#ifdef LOGXCFG_SYNC
			struct _sync_creator : public core::_msg_creator
			{
				_sync_creator(std::unique_lock<std::mutex>&& _mutex, core_impl* _core_impl)
					: mutex(std::move(_mutex))
					, core_impl(_core_impl)
				{
				}

				virtual ~_sync_creator() override
				{
					::operator delete(msg_mem);
				}

				virtual void done(details::message_base* msg) override
				{
					core_impl->_send_sink(msg);
					core_impl->mSyncCreator.destruct();
				}

				core_impl* core_impl;
				std::unique_lock<std::mutex> mutex;
			};

			virtual core::_msg_creator* _get_creator(std::size_t _msg_size) override
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
			std::unordered_map<std::type_index, std::shared_ptr<tag>> mDefaultTags;

			std::mutex mSinkMutex;
			std::mutex mMsgMutex;

			std::thread mWorkerThread;
			bool mRunning;

#ifdef LOGXCFG_SYNC
			details::uninitialized<_sync_creator> mSyncCreator;
#endif
		};
	}



	// core stuff
	impl::core_impl GLCore;

	core::~core()
	{
	}

	core& core::get_core()
	{
		return GLCore;
	}
}