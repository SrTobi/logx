#pragma once
#ifndef _LOGX_LOG_
#define _LOGX_LOG_

#include <memory>
#include "core.hpp"

namespace logx {

	class _logx_helper_ns
	{
	public:
		template<typename _MsgTy, typename... _Args>
		void log(_MsgTy&& _msg, _Args&&... _args)
		{
			details::log_core<>::get_core()
				.add_log_message(std::forward<_MsgTy>(_msg), std::forward<_Args>(_args)...);
		}
	};

	class logger: public _logx_helper_ns
	{

	};

	template<typename _MsgTy, typename... _Args>
	void log(_MsgTy&& _msg, _Args&&... _args)
	{
		details::log_core<>::get_core()
			.add_log_message(std::forward<_MsgTy>(_msg), std::forward<_Args>(_args)...);
	}

}



#endif