#include <mutex>
#include <iostream>
#include <thread>
#include <array>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <utility>

#include <boost/lockfree/queue.hpp>

#include "logx/core.hpp"
#include "logx/sink.hpp"
#include "logx/tag.hpp"
#include "logx/text_sink.hpp"
#include <logx/logx_api.hpp>

#include "sink_message.hpp"
#include "memory.hpp"

namespace logx {

	namespace impl {

		struct finally
		{
			template<typename Handler>
			finally(Handler handler)
				: mHandler(handler)
			{
			}

			~finally()
			{
				mHandler();
			}

		private:
			std::function<void()> mHandler;
		};


		class core_impl final : public core
		{
		public:
			core_impl()
				: mMessageQueue(0)
			{
				std::atexit(&core_impl::_global_exit);
				auto sink = std::make_shared<text_sink>(std::cout);

				mSinks.push_back(std::bind(&text_sink::on_message, sink, std::placeholders::_1));
			}

			virtual ~core_impl() override
			{
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
				if (!mIsRunning && mShouldRun)
				{
					mWorkerThread = std::thread(std::bind(&core_impl::_worker_run, this));

					// spinlock until worker is running
					while (!mIsRunning && mShouldRun) {}
				}

			}

			virtual void exit(bool force) override
			{
				if (mIsRunning)
				{
					mShouldFlush = true;
					if (force)
						mShouldRun = false;

					while (mIsRunning)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
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
				finally fin = [this]()
				{
					mIsRunning = false;
				};

				while (mShouldRun)
				{
					details::message_base* msg;

					int spin_locked = 0;
					while (!mMessageQueue.pop(msg))
					{
						if (mShouldFlush)
							return;

						if (spin_locked > 100)
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(std::min(spin_locked / 100, 500)));
						}
						else{
							std::this_thread::yield();
						}
						++spin_locked;
						
					}

					std::lock_guard<std::mutex> lock(mSinkMutex);
					sink_message_impl sink_msg(msg, mDefaultTags);

					for (auto& sink : mSinks)
					{
						sink(sink_msg);
					}
				}
			}


			virtual void* _allocate_message(std::size_t _size) override
			{
				return ::operator new(_size);
			}

			virtual void _post_message(details::message_base* message) override
			{
				if(!mShouldFlush)
					mMessageQueue.push(message);
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
			std::atomic<bool> mShouldRun = true;
			std::atomic<bool> mIsRunning = false;
			std::atomic<bool> mShouldFlush = false;

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