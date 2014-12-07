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

#ifndef LOGX_NO_LOG_MARCOS

struct logx_call_helper
{
	static inline logx::details::stream_pack<> log()
	{
		return ::logx::log();
	}

	static inline logx::details::stream_pack_end<> dummy()
	{
		return ::logx::details::stream_pack_end<>();
	}
};






#define logxRAWLOG(...)		logx_call_helper::log() << __VA_ARGS__ << ::logx::end
#define logxLOG(...)		logxRAWLOG(__VA_ARGS__)[::logx::tags::time(), logxSOURCE, ::logx::tags::thread()]

#ifdef LOGX_PUBLISH
#	define logxTRACE(...)	logx_call_helper::dummy()
#else
#	define logxTRACE(...)	logxLOG(__VA_ARGS)[trace]
#endif

#define logxDEBUG(...)		logxLOG(__VA_ARGS__)[::logx::tags::debug]
#define logxDETAIL(...)		logxLOG(__VA_ARGS__)[::logx::tags::detail]
#define logxINFO(...)		logxLOG(__VA_ARGS__)[::logx::tags::info]
#define logxWARN(...)		logxLOG(__VA_ARGS__)[::logx::tags::warning]
#define logxERROR(...)		logxLOG(__VA_ARGS__)[::logx::tags::error]
#define logxFATAL(...)		logxLOG(__VA_ARGS__)[::logx::tags::fatal]



#endif

#endif
