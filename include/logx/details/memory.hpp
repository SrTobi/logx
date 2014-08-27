#pragma once
#ifndef _LOGX_MEMORY_HPP
#define _LOGX_MEMORY_HPP

#include <stdexcept>
#include "config.hpp"


namespace logx {
	namespace details {


		template<typename _Ty>
		class uninitialized
		{
		public:
			~uninitialized()
			{
				if (mInitialized)
					destruct();
			}


			template<typename... Args>
			_Ty* construct(Args&&... _args)
			{
#ifdef LOGX_DEBUG
				if (mInitialized)
				{
					throw std::logic_error("already initialized");
				}
#endif
				mInitialized = true;
				return new (mBuffer.data()) _Ty(std::forward<Args>(_args)...);
			}

			void destruct()
			{
				get()->~_Ty();
				mInitialized = false;
			}

			const _Ty* get() const
			{
#ifdef LOGX_DEBUG
				if (!mInitialized)
				{
					throw std::logic_error("not initialized");
				}
#endif
				return reinterpret_cast<const _Ty*>(mBuffer.data());
			}

			_Ty* get()
			{
				return const_cast<_Ty*>(
						const_cast<const uninitialized*>(this)->get()
					);
			}

			const _Ty* operator ->() const
			{
				return get();
			}

			_Ty* operator ->()
			{
				return get();
			}

		private:

			std::array<char, sizeof(_Ty)> mBuffer;
			bool mInitialized = false;
		};







	}
}





#endif