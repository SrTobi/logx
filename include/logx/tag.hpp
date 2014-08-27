#pragma once
#ifndef _LOGX_TAG_HPP
#define _LOGX_TAG_HPP



#include <string>
#include <ostream>

namespace logx {



	class tag_base
	{
	public:
		virtual ~tag_base() {}
		virtual std::wstring name() const = 0;
		virtual std::wstring value() const = 0;
	};


	template<typename... Args>
	class parameterized_tag_base : public tag_base
	{
	protected:
		typedef std::tuple<const Args...> tuple_type;
	public:
		parameterized_tag_base(Args&&... _args)
			: mArgs(std::forward<Args>(_args)...)
		{}

		virtual std::wstring value() const override
		{
			return _build_value(mArgs);
		}

	protected:
		virtual std::wstring _build_value(const tuple_type& _args) const = 0;

	private:
		const tuple_type mArgs;
	};


	template<typename C>
	std::basic_ostream<C>& operator <<(std::basic_ostream<C>& _stream, const tag_base& _tag)
	{
		_stream << _tag.value();
		return _stream;
	}


	template<typename NameTag>
	struct tag;

#define logxTAG(_name, _val_build, ...)																				\
	template<>																										\
	struct logx::tag<struct NameTag##_name> : public logx::parameterized_tag_base<__VA_ARGS__>						\
	{																												\
		template<typename... Args> tag(Args&&... _args) : parameterized_tag_base(std::forward<Args>(_args)...){}	\
		virtual std::wstring name() const override { return LOGXW(#_name); }										\
		virtual std::wstring _build_value(const tuple_type& _args) const override									\
						{ return _val_build; }																		\
	};	\
	typedef logx::tag<struct NameTag##_name> _name;
}






#endif