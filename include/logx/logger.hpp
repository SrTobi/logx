#pragma once
#ifndef _LOGX_LOGGER_HPP
#define _LOGX_LOGGER_HPP

#include <tuple>
#include "config.hpp"
#include "details/list_message.hpp"


namespace logx {
	namespace details {
		template<typename... _Args>
		struct tag_annotation_pack
		{
			tag_annotation_pack(std::tuple<_Args...>&& _args)
				: mValues(_args)
			{}

			template<typename T>
			tag_annotation_pack<_Args..., T> operator ,(T&& _val)
			{
				return tag_annotation_pack<_Args..., T>(std::tuple_cat(std::move(mValues), std::forward_as_tuple(_val)));
			}

			std::tuple<_Args...> mValues;
		};

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

			template<typename... _AArgs>
			stream_pack_end<_Args..., _AArgs...> operator [](tag_annotation_pack<_AArgs...>&& _pack)
			{
				return stream_pack_end<_Args..., _AArgs...>(std::tuple_cat(mArgs, _pack.mValues));
			}

			std::tuple<_Args...> mArgs;
		};


		template<typename PrePack = std::tuple<>, typename... _Args>
		struct stream_pack
		{
			stream_pack(PrePack _prePack)
				: mPrePack(std::move(_prePack))
			{}

			stream_pack(PrePack _prePack, std::tuple<_Args...> _args)
				: mArgs(std::move(_args))
				, mPrePack(std::move(_prePack))
			{
			}

			template<typename NewA>
			stream_pack<PrePack, _Args..., NewA> operator << (NewA&& _newa)
			{
				return stream_pack<PrePack, _Args..., NewA>(mPrePack, std::tuple_cat(std::move(mArgs), std::forward_as_tuple(_newa)));
			}

			template<typename... _AArgs>
			void operator << (stream_pack_end<_AArgs...>&& _streamp)
			{
				auto ma = std::tuple_cat(std::move(mArgs), std::move(mPrePack), std::move(_streamp.mArgs));
				_send_message(ma);
			}

		private:
			template<typename... _AArgs>
			void _send_message(std::tuple<_AArgs...> _msg_args) const
			{
				core::get_core()
					.add_log_message<details::list_message<sizeof...(_Args), _AArgs...>>(std::move(_msg_args));
			}

			PrePack mPrePack;
			std::tuple<_Args...> mArgs;
		};
	}


	template<typename Arg>
	details::uncopyable_element_wrapper<Arg> nocp(Arg& _arg)
	{
		return details::uncopyable_element_wrapper < Arg > {_arg};
	}

	inline details::stream_pack<> log()
	{
		return details::stream_pack<>(std::tuple<>());
	}

	static details::stream_pack_end<> end;



	template<typename... _Args>
	class logger
	{
	public:
		logger(_Args&&... _args)
			: mArgs(std::forward<_Args>(_args)...)
		{
		}

		logger(std::tuple<_Args...>&& _args)
			: mArgs(std::move(_args))
		{
		}

		inline details::stream_pack<std::tuple<_Args...>> log()
		{
			return details::stream_pack<std::tuple<_Args...>>(mArgs);
		}

		inline details::stream_pack_end<> dummy()
		{
			return ::logx::details::stream_pack_end<>();
		}

	private:
		const std::tuple<_Args...> mArgs;
	};



}

#endif