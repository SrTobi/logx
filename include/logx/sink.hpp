#pragma once
#ifndef _LOGX_SINK_HPP
#define _LOGX_SINK_HPP

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
	private:
	};
}




#endif