#pragma once
#ifndef _LOGX_LOG_
#define _LOGX_LOG_

#include <memory>
#include "details/stream_packs.hpp"
#include "details/list_message.hpp"
#include "core.hpp"

namespace logx {

	template<typename Arg>
	details::uncopyable_element_wrapper<Arg> nocp(Arg& _arg)
	{
		return details::uncopyable_element_wrapper < Arg > {_arg};
	}

	inline details::stream_pack<> log()
	{
		return details::stream_pack<>();
	}

	static details::stream_pack_end<> end;
}



#endif
