#include <mutex>
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <utility>

#include <boost/lockfree/queue.hpp>
#include <utilx/finally.hpp>

#include "logx/core.hpp"
#include "logx/sink.hpp"
#include "logx/tag.hpp"
#include "logx/sinks/text_sink.hpp"
#include <logx/logx_api.hpp>
#include <boost/assert.hpp>

#include "sink_message.hpp"
#include "memory.hpp"

namespace logx {

	namespace impl {

		class core_impl final : public core
		{
		public:
			core_impl()
				: mMessageQueue(0)
				, mShouldRun(false)
				, mIsRunning(false)
				, mShouldFlush(false)
				, mInits(0)
			{
				std::atexit(&core_impl::_global_exit);
				auto sink = std::make_shared<text_sink>(std::cout);

				mSinks.push_back(std::bind(&text_sink::on_message, sink, std::placeholders::_1));
			}

			virtual ~core_impl() override
			{
				if(mWorkerThread.joinable())
					mWorkerThread.detach();
			}


			virtual void add_sink(sink _sink) override
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				mSinks.emplace_back(_sink);
			}

			virtual void remove_all_sinks() override
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				mSinks.clear();
			}


			virtual bool remove_default_tag(const std::type_info& _ty) override
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				return mDefaultTags.erase(_ty) > 0;
			}

			virtual void init() override
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				mShouldRun = true;
				mShouldFlush = false;
				if (!mIsRunning)
				{
					mWorkerThread = std::thread(std::bind(&core_impl::_worker_run, this));

					// spinlock until worker is running
					while (!mIsRunning) {}
				}

				++mInits;
			}

			virtual void exit(bool force) override
			{
				{
					std::lock_guard<std::mutex> lock(mSinkMutex);
					if (--mInits > 0)
						return;
					
					// mInits should not be < 0
					BOOST_ASSERT(mInits >= 0);
					mInits = 0;

					if (mIsRunning)
					{
						mShouldFlush = true;
						if (force)
							mShouldRun = false;
					}
				}

				while (mIsRunning)
				{
					std::this_thread::yield();
				}
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

			void _worker_run()
			{
				mIsRunning = true;
				utilx::finally fin = [this]()
				{
					mIsRunning = false;
				};

				while (mShouldRun)
				{
					details::message_base* msg;

					unsigned int spin_locked = 0;
					while (!mMessageQueue.pop(msg))
					{

						if (spin_locked > 100)
						{
							if(mShouldFlush)
								return;
							std::this_thread::sleep_for(std::chrono::milliseconds(std::min(spin_locked / 100, 100u)));
						}
						else{
							std::this_thread::yield();
						}
						++spin_locked;
						
					}

					_send_message_to_sink(msg);
				}
			}

			void _send_message_to_sink(details::message_base* msg)
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				sink_message_impl sink_msg(msg, mDefaultTags);

				for (auto& sink : mSinks)
				{
					try
					{
						sink(sink_msg);
					}
					catch (...)
					{
						// ignore exceptions and go on
					}
				}
				delete msg;
			}


			virtual void* _allocate_message(std::size_t _size) override
			{
				return ::operator new(_size);
			}

			virtual void _post_message(details::message_base* message) override
			{
				if (mIsRunning && !mShouldFlush)
				{
					mMessageQueue.push(message);
				}
				else{
					_send_message_to_sink(message);
				}
			}

			static void _global_exit();
			void _exit()
			{
				mShouldFlush = true;
			}
		private:
			std::vector<sink> mSinks;
			std::unordered_map<std::type_index, std::shared_ptr<tag>> mDefaultTags;

			std::mutex mSinkMutex;
			std::thread mWorkerThread;
			std::atomic<bool> mShouldRun;
			std::atomic<bool> mIsRunning;
			std::atomic<bool> mShouldFlush;
			std::atomic<int> mInits;

			boost::lockfree::queue<details::message_base*> mMessageQueue;
		};
	}



	// core
	impl::core_impl GLCore;

	core::~core()
	{
	}

	core& core::get_core()
	{
		return GLCore;
	}

	void impl::core_impl::_global_exit()
	{
		GLCore._exit();
	}
}
