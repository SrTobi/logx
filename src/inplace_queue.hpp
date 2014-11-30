#pragma once
#ifndef _LOGX_INPLACE_QUEUE_HPP
#define _LOGX_INPLACE_QUEUE_HPP



#include <string>
#include <type_traits>
#include <memory>
#include <utility>
#include <logx/logx_api.hpp>


namespace logx {

	namespace impl {


		class inplace_queue_base
		{
		public:

			void* begin_push(std::size_t _size);
			void end_push();

			void* begin_pop();
			void* begin_pop_wait();
			void end_pop();

		private:
		};

		template<typename Base>
		class inplace_queue : private inplace_queue_base
		{
		public:
			struct push_deleter
			{
				push_deleter(inplace_queue_base* base)
					: base(base)
				{
				}

				void operator()(void*)
				{
					base->end_push();
				}

			private:
				inplace_queue_base* base;
			};
		public:
			template<typename T, typename... Args>
			std::unique_ptr<T, push_deleter> emplace(Args&&... _args)
			{
				return std::unique_ptr<T, push_deleter>
					(
						new(
							begin_push(sizeof(T))
						)T(
							std::forward<Args>(_args)...
						)
						,
						push_deleter(this)
					);
			}


		private:
			
		};



	}
}





#endif