#include <mutex>
#include <iostream>
#include <thread>
#include <array>
#include <unordered_map>
#include <typeindex>
#include <functional>


#if defined(WIN32) && !defined(_WIN32_WINNT)
#	define _WIN32_WINNT 0x0501
#endif
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
namespace asio = boost::asio;

#include "logx/core.hpp"
#include "logx/core_service.hpp"
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
				: mService(&mOwnService)
			{
				auto sink = std::make_shared<text_sink>(std::cout);

				mSinks.push_back(std::bind(&text_sink::on_message, sink, std::placeholders::_1));
			}

			virtual ~core_impl() override
			{
				_stop();
			}


			virtual void add_sink(sink _sink) override
			{
				std::lock_guard<std::mutex> lock(mSinkMutex);
				mSinks.push_back(_sink);
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

			virtual unsigned int async_count() const
			{
				return 0;
			}

			virtual void async_count(unsigned int _c)
			{
			}




			asio::io_service& service()
			{
				return *mService;
			}

			void reset_service(asio::io_service* _service)
			{
				if (!_service)
				{
					_service = &mOwnService;
				}

				if (_service == mService)
				{
					return;
				}

				// reset service
				assert(0);
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
				std::lock_guard<std::mutex> lock(mSinkMutex);

				/*if (mServiceWork)
					return;

				mServiceWork.reset(new asio::io_service::work(mService));

				auto process = [this](){ mService.run(); };
				//mWorkerThread = std::thread(std::move(process));*/
			}

			void _stop()
			{
				//mServiceWork.reset();
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


			virtual void* _allocate_message(std::size_t _size) override
			{
				return ::operator new(_size);
			}

			virtual void _post_message(details::message_base* message) override
			{
				if (true /* sync ? */)
				{
					_send_sink(message);

				}
				else
				{

				}

				service().post(std::bind(&core_impl::_send_sink, this, message));
			}

		private:
			std::vector<sink> mSinks;
			std::unordered_map<std::type_index, std::shared_ptr<tag>> mDefaultTags;

			std::mutex mSinkMutex;
			std::mutex mMsgMutex;

			asio::io_service* mService;
			asio::io_service mOwnService;
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


	// core service
	core_service::core_service(boost::asio::io_service& _service)
		: mService(_service)
	{
		GLCore.reset_service(&_service);
	}

	core_service::~core_service()
	{
		if(active())
			GLCore.reset_service(nullptr);
	}

	bool core_service::active() const
	{
		return &(GLCore.service()) == &mService;
	}
}