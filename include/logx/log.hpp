#pragma once
#ifndef _LOGX_LOG_
#define _LOGX_LOG_

#include <memory>
#include "core.hpp"

namespace logx {


	template<typename _MsgTy, typename... _Args>
	void log(_MsgTy&& _msg, _Args&&... _args)
	{
		core::get_core()
			.add_log_message(std::forward<_MsgTy>(_msg), std::forward<_Args>(_args)...);
	}

}



#endif