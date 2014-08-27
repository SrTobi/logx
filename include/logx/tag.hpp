#pragma once
#ifndef _LOGX_TAG_HPP
#define _LOGX_TAG_HPP



#include <string>
#include <ostream>

namespace logx {



	class tag
	{
	public:
		virtual ~tag() {}
		virtual std::wstring name() const = 0;
		virtual std::wstring value() const = 0;
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
	std::basic_ostream<C>& operator <<(std::basic_ostream<C>& _stream, const tag& _tag)
	{
		_stream << _tag.value();
		return _stream;
	}

#define logxTAG(_name, _val_build, ...)																				\
	struct _name : public logx::parameterized_tag_base<__VA_ARGS__>						\
	{																												\
		template<typename... Args> _name(Args&&... _args) : logx::parameterized_tag_base<__VA_ARGS__>(std::forward<Args>(_args)...){}	\
		inline virtual std::wstring name() const override { return LOGXW(#_name); }										\
		inline virtual std::wstring _build_value(const tuple_type& _args) const override									\
		{ return _val_build; }																		\
	};
}






#endif