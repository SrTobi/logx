#pragma once
#ifndef _TAG_ANNOTATION_PACK_HPP
#define _TAG_ANNOTATION_PACK_HPP

#include <tuple>



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


		template<typename... _Args>
		struct stream_pack
		{
			stream_pack()
			{}

			stream_pack(std::tuple<_Args...>&& _args)
				: mArgs(std::move(_args))
			{
			}

			template<typename NewA>
			stream_pack<_Args..., NewA> operator << (NewA&& _newa)
			{
				return stream_pack<_Args..., NewA>(std::tuple_cat(std::move(mArgs), std::forward_as_tuple(_newa)));
			}

			template<typename... _AArgs>
			void operator << (stream_pack_end<_AArgs...>&& _streamp)
			{
				auto ma = std::tuple_cat(std::move(mArgs), std::move(_streamp.mArgs));
				core::get_core()
					.add_log_message<details::list_message<sizeof...(_Args), _Args..., _AArgs...>>(std::move(ma));
			}

			std::tuple<_Args...> mArgs;
		};
	}

}


#endif