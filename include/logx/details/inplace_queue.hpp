#pragma once
#ifndef _LOGX_INPLACE_QUEUE_HPP
#define _LOGX_INPLACE_QUEUE_HPP



#include <string>
#include <type_traits>
#include <memory>
#include <logx/logx_api.hpp>


namespace logx {

	namespace details{

		template<typename Base>
		class inplace_queue_interface
		{
		public:
			template<typename T, typename... Args>
			std::unique_ptr<T> emplace(Args&&... _args)
			{
				auto deleter = [this](void*) { this->end_allocate(); }

				return std::unique_ptr<T, decltype(deleter)>
					(
						new(
							begin_allocate(sizeof(T))
						)(
							std::forward<Args>(_args)...
						)
						,
						deleter
					);
			}

		protected:
			virtual void* begin_allocate(std::size_t _size) = 0;
			virtual void end_allocate() = 0;
		};



	}
}





#endif