#pragma once
#ifndef _LOGX_SINK_HPP
#define _LOGX_SINK_HPP

#include "details/message.hpp"
#include "core.hpp"

namespace logx {

	class sink_message
	{
	public:
		virtual const string& msg() const = 0;

	private:
	};

}




#endif