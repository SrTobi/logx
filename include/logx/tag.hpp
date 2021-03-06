#pragma once
#ifndef _LOGX_TAG_HPP
#define _LOGX_TAG_HPP



#include <string>
#include <ostream>
#include "config.hpp"
#include "logx/logx_api.hpp"

namespace logx {



	class LOGX_EXPORT tag
	{
	public:
		virtual ~tag() {}
		virtual string name() const = 0;
		virtual string value() const = 0;

	};


	template<typename... Args>
	class parameterized_tag_base : public tag
	{
	protected:
		typedef std::tuple<const Args...> tuple_type;
	public:
		parameterized_tag_base(Args... _args)
			: mArgs(std::move(_args)...)
		{}

		virtual string value() const override
		{
			return _build_value();
		}

	protected:
		virtual string _build_value() const = 0;

		template<size_t I>
		const typename std::tuple_element<I, tuple_type>::type& arg() const
		{
			return std::get<I>(mArgs);
		}

	private:
		const tuple_type mArgs;
	};


	template<typename C>
	std::basic_ostream<C>& operator <<(std::basic_ostream<C>& _stream, const tag& _tag)
	{
		_stream << _tag.value();
		return _stream;
	}

#define logxTAG_PACK_OPERATOR(_name)																									\
	template<typename T> ::logx::details::tag_annotation_pack<_name, T> operator, (_name&& _val, T&& _add)								\
	{ return ::logx::details::tag_annotation_pack<_name, T>(std::make_tuple(std::move(_val), std::forward<T>(_add))); }			

#define logxTAG_BEGIN(_name, _val_build, ...)																							\
	struct _name : public logx::parameterized_tag_base<__VA_ARGS__>																		\
	{																																	\
		template<typename... Args> _name(Args&&... _args) : logx::parameterized_tag_base<__VA_ARGS__>(std::forward<Args>(_args)...){}	\
		inline virtual string name() const override { return #_name; }																	\
		inline virtual string _build_value() const override														\
		{ return _val_build; }

#define logxTAG_END(_name)																												\
	};																																	\
	logxTAG_PACK_OPERATOR(_name)

#define logxTAG(_name, _val, ...)				\
	logxTAG_BEGIN(_name, _val, __VA_ARGS__)		\
	logxTAG_END(_name)
}






#endif