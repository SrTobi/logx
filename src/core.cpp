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
#include <atomic>

#include "sink_message.hpp"
#include "future_strand.hpp"
#include "memory.hpp"

namespace logx {

	namespace impl {

		class core_impl final : public core
		{
		public:
			core_impl()
				: mService(nullptr)
				, mSynchronous(true)
				, mPipelineState(PipelineState::Stoped)
			{
				auto sink = std::make_shared<text_sink>(std::cout);

				add_sink(std::bind(&text_sink::on_message, sink, std::placeholders::_1));
			}

			virtual ~core_impl() override
			{
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

			virtual unsigned int async_count() const override
			{
				return 0;
			}

			virtual void async_count(unsigned int _c)
			{
			}


			virtual bool synchronous() const
			{
				return mSynchronous;
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


			/*void _notify_message(log_thread& _thread)
			{
				//while (_thread.mMessages.size())
				//{
				//	auto msg = std::move(_thread.mMessages.front());
				//	_send_sink(msg.get());
				//	_thread.mMessages.pop_front();
				//}
			}*/

			void _send_sink_synchronous(details::message_base* msg)
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

			/************************************** Message pipeline **************************************/
			enum class PipelineState
			{
				Running,
				Stoped,
				Ending
			};

			void _start_message_pipeline()
			{
				if (!synchronous())
				{
					return;
				}
				assert(mService);
				mMakeMessageStrand.reset(new details::future_strand(*mService));
				mSinkStrands.clear();
				for (unsigned int i = 0; i < mSinks.size(); ++i)
					mSinkStrands.emplace_back(*mService);

				mSynchronous = false;
			}

			std::unique_lock<std::mutex> _end_message_pipeline()
			{
				std::lock_guard<std::mutex> lck(mEmptyPipelineMutex);

			}

			virtual void _post_message(details::message_base* message) override
			{
				if(mPipelineState != )


				auto handler = mMakeMessageStrand->wrap([this](sink_message_impl* msg)
				{
					_send_message_to_sink(0, msg);
				});
					
				mService->post(
				[this, message, handler]{
					_make_sink_messsage(message, handler);
				});
			}


			template<typename Handler>
			void _make_sink_messsage(details::message_base* message, Handler handler)
			{
				sink_message_impl* msg = new sink_message_impl(message, mDefaultTags);

				handler(std::move(msg));
			}

			void _send_message_to_sink(unsigned int sink_idx, sink_message_impl* msg)
			{
				if (sink_idx < mSinks.size())
				{
					auto& strand = mSinkStrands.at(sink_idx);

					strand.dispatch(std::bind(&core_impl::_do_send_message_to_sink, this, sink_idx, std::move(msg)));
				}
				else{
					delete msg;
				}
			}

			void _do_send_message_to_sink(unsigned int sink_idx, sink_message_impl* msg)
			{
				auto& sink = mSinks.at(sink_idx);
				sink(*msg);

				_send_message_to_sink(sink_idx + 1, msg);
			}

		private:
			std::atomic<PipelineState> mPipelineState;
			std::mutex mEmptyPipelineMutex;
			std::unique_ptr<details::future_strand> mMakeMessageStrand;
			std::vector<asio::io_service::strand> mSinkStrands;


			/************************************** Message pipeline end **************************************/

		private:
			std::vector<sink> mSinks;
			std::unordered_map<std::type_index, std::shared_ptr<tag>> mDefaultTags;

			bool mSynchronous;
			std::mutex mSyncMutex;
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
		return 0;
	}
}