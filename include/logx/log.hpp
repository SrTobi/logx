#pragma once
#ifndef _LOGX_LOG_
#define _LOGX_LOG_

#include <memory>
#include "details/list_message.hpp"
#include "core.hpp"

namespace logx {

	template<typename Arg>
	details::uncopyable_element_wrapper<Arg> nocp(Arg& _arg)
	{
		return details::uncopyable_element_wrapper < Arg > {_arg};
	}

	template<typename... _Args>
	void log(_Args&&... _args)
	{
		core::get_core()
			.add_log_message<details::list_message<sizeof...(_Args), _Args...>>(std::forward<_Args>(_args)...);
	}

	namespace details {

		template<typename... _Args>
		struct stream_pack_end
		{
			stream_pack_end()
			{}

			stream_pack_end(std::tuple<_Args...> _args)
				: mArgs(std::move(_args))
			{
			}

			template<typename NewA>
			stream_pack_end<_Args..., NewA> operator [](NewA&& _newa)
			{
				return stream_pack_end<_Args..., NewA>(std::tuple_cat(std::move(mArgs), std::forward_as_tuple(_newa)));
			}

			std::tuple<_Args...> mArgs;
		};


		template<typename... _Args>
		struct stream_pack
		{
			stream_pack()
			{}

			stream_pack(std::tuple<_Args...> _args)
				: mArgs(std::move(_args))
			{
			}

			template<typename NewA>
			stream_pack<_Args..., NewA> operator << (NewA&& _newa)
			{
				return stream_pack<_Args..., NewA>(std::tuple_cat(std::move(mArgs), std::forward_as_tuple(_newa)));
			}

			template<typename... _AArgs>
			void operator << (stream_pack_end<_AArgs...>& _streamp)
			{
				auto ma = std::tuple_cat(std::move(mArgs), std::move(_streamp.mArgs));
				core::get_core()
					.add_log_message<details::list_message<sizeof...(_AArgs), decltype(ma)>>(std::move(ma));
			}

			std::tuple<_Args...> mArgs;
		};
	}

	inline details::stream_pack<> logs()
	{
		return details::stream_pack<>();
	}

	static details::stream_pack_end<> end;
}



#endif
