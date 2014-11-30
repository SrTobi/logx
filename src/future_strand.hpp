#pragma once
#ifndef _FUTURE_STRAND_HPP
#define _FUTURE_STRAND_HPP

#if defined(WIN32) && !defined(_WIN32_WINNT)
#	define _WIN32_WINNT 0x0501
#endif
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>


namespace logx {
	namespace details {


		class future_strand
		{
		private:
			typedef boost::asio::io_service io_service;
			typedef std::function<void()> task_type;


			struct future_handler
			{
				future_handler(future_strand* _strand)
					: strand(_strand)
				{
				}

				~future_handler()
				{

				}

				std::shared_ptr<future_handler> next;
				task_type task;
				future_strand* strand;
			};

			typedef std::shared_ptr<future_handler> fh_ptr;

		public:

			template<typename Handler>
			class wrap_handler
			{
			public:
				wrap_handler(Handler _handler, fh_ptr _fh)
					: mHandler(std::move(_handler))
					, mFutureHandler(std::move(_fh))
				{
				}

				template<typename... Args>
				void operator()(Args&&... _args)
				{
					assert(mFutureHandler);
					assert(!mFutureHandler->task);
					
					mFutureHandler->task = std::bind(mHandler, std::forward<Args>(_args)...);
					
					// Save strand but reset Handler, so flush can release shared_ptr's iteratively
					auto* strand = mFutureHandler->strand;
					mFutureHandler = nullptr;
					strand->_flush();
				}

			private:
				fh_ptr mFutureHandler;
				Handler mHandler;
			};
		public:
			future_strand(io_service& _service)
				: mStrand(_service)
			{
				mFirstHandler = mLastHandler = std::make_shared<future_handler>(this);
			}

			template<typename Handler>
			auto wrap(Handler _handler) -> wrap_handler<Handler>
			{
				auto fh = std::make_shared<future_handler>(this);

				auto prev = std::atomic_exchange(&mLastHandler, fh);

				assert(!prev->next);
				prev->next = fh;

				return wrap_handler<Handler>(std::forward<Handler>(_handler), fh);
			}

			template<typename Handler>
			void dispatch(Handler _handler)
			{
				wrap(std::forward<Handler>(_handler))();
			}

		private:
			void _flush()
			{
			again:
				auto first = std::atomic_exchange(&mFirstHandler, fh_ptr());

				// If first is zero, another thread is executing flush
				if (!first)
					return;


				while(true)
				{
					auto& next = first->next;

					// there is no next node -> exit
					if (!next || !next->task)
					{
						break;
					}

					// if there is a task, dispatch it 
					mStrand.dispatch(std::move(next->task));
					first = std::move(next);
				}

				mFirstHandler = first;
				auto next = first->next;
				if (next && next->task)
					goto again;
			}

		private:
			fh_ptr mFirstHandler;
			fh_ptr mLastHandler;
			io_service::strand mStrand;
		};



	}
}




#endif