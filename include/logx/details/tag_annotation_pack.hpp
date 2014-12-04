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

	}

}


#endif