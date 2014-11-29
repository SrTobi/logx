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
#include "logx/details/formatter.hpp"
#include "logx/details/memory.hpp"
#include "logx/details/message.hpp"
#include "logx_api.hpp"

namespace logx {

	namespace details {
		class LOGX_EXPORT sink_message_impl : public sink_message
		{
		public:
			sink_message_impl(message_base* message, const std::unordered_map<std::type_index, std::shared_ptr<tag>>& _default_tags)
				: mMessage(message)
			{
				mArgs = mMessage->args();

				mArgsAsString.reserve(mArgs.size());
				for (auto arg : mArgs)
				{
					mArgsAsString.emplace_back(arg->to_string());
					
					auto* asTag = arg->as_tag();
					if (asTag)
						mTags.emplace(typeid(*asTag), asTag);
				}

				for (auto& e : _default_tags)
				{
					if (!mTags.count(e.first))
					{
						mTags.emplace(e.first, e.second.get());
					}
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
				if (mTagList.size() != mTags.size())
				{
					for (auto& e : mTags)
					{
						mTagList.push_back(e.second);
					}
				}
				return mTagList;
			}

			virtual const tag* _get_tag(const std::type_info& _ty) const override
			{
				auto it = mTags.find(_ty);
				return it == mTags.end() ? nullptr : it->second;
			}

		private:
			message_base* mMessage;
			string mDescription;
			std::vector<const message_arg*> mArgs;
			std::vector<string> mArgsAsString;
			mutable std::vector<const tag*> mTagList;
			std::unordered_map<std::type_index, const tag*> mTags;
		};

		class LOGX_EXPORT log_core_impl : public core
		{
		public:
			log_core_impl()
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


			void _notify_message(log_thread& _thread)
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
				_sync_creator(std::unique_lock<std::mutex>&& _mutex, log_core_impl* _core_impl)
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
			uninitialized<_sync_creator> mSyncCreator;
#endif
		};
	}

	std::shared_ptr<core> core::GLogCore = std::make_shared<details::log_core_impl>();

	core& core::get_core()
	{
		return *GLogCore;
	}

	void core::set_core(core& _core)
	{
		GLogCore = _core.shared_from_this();
	}
}