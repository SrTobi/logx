#pragma once
#ifndef _LOGX_LOG_
#define _LOGX_LOG_

#include <memory>
#include "details/list_message.hpp"
#include "core.hpp"

namespace logx {


	template<typename... _Args>
	void log(_Args&&... _args)
	{
		core::get_core()
			.add_log_message<details::list_message<sizeof...(_Args), _Args...>>(std::forward<_Args>(_args)...);
	}

	namespace details {

		struct stream_pack_end
		{};


		template<typename... _Args>
		struct stream_pack
		{
		public:
			stream_pack()
			{}

			stream_pack(std::tuple<_Args...> _args)
				: mArgs(std::move(_args))
			{
			}

			template<typename NewA>
			stream_pack<_Args..., NewA> operator << (NewA _newa)
			{
				return stream_pack<_Args..., NewA>(std::tuple_cat(std::move(mArgs), std::forward_as_tuple(_newa)));
			}

			void operator << (stream_pack_end&)
			{
				core::get_core()
					.add_log_message<details::list_message<sizeof...(_Args), _Args...>>(std::move(mArgs));
			}

			std::tuple<_Args...> mArgs;
		};
	}

	inline details::stream_pack<> logs()
	{
		return details::stream_pack<>();
	}

	static details::stream_pack_end end;
}



#endif