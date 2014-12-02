#pragma once
#ifndef _LOGX_LOG_
#define _LOGX_LOG_

#include <memory>
#include "details/list_message.hpp"
#include "core.hpp"

namespace logx {


	template<typename... _Args>
	void log(_Args&&... _args)
	{
		core::get_core()
			.add_log_message<details::list_message<sizeof...(_Args), _Args...>>(std::forward<_Args>(_args)...);
	}

}



#endif