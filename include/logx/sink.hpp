#pragma once
#ifndef _LOGX_SINK_HPP
#define _LOGX_SINK_HPP

#include <typeinfo>
#include "details/message.hpp"
#include "core.hpp"

namespace logx {

	class tag;

	class sink_message
	{
	public:
		virtual ~sink_message() {}
		virtual const string& msg() const = 0;
		virtual const std::vector<string>& args() const = 0;
		virtual const std::vector<const tag*>& tags() const = 0;
		
		template<typename T>
		const T* get_tag() const
		{
			const tag* _tag = _get_tag(typeid(T));
			return _tag ? dynamic_cast<const T*>(_tag) : nullptr;
		}
	protected:
		virtual const tag* _get_tag(const std::type_info& _ty) const = 0;
	};
}




#endif