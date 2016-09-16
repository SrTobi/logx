#pragma once
#ifndef _LOGX_LOG_
#define _LOGX_LOG_

#include <memory>
#include "details/list_message.hpp"
#include "core.hpp"
#include "logger.hpp"

#ifndef LOGX_NO_LOG_MARCOS



#define logxRAWLOG(...)		logx::log() << __VA_ARGS__ << ::logx::end
#define logxLOG(...)		logxRAWLOG(__VA_ARGS__)[::logx::tags::time(), logxSOURCE, ::logx::tags::thread()]

#ifdef LOGX_PUBLISH
#	define logxTRACE(...)	logx::dummy()
#else
#	define logxTRACE(...)	logxLOG(__VA_ARGS)[::logx::tags::trace]
#endif

#define logxDEBUG(...)		logxLOG(__VA_ARGS__)[::logx::tags::debug]
#define logxDETAIL(...)		logxLOG(__VA_ARGS__)[::logx::tags::detail]
#define logxINFO(...)		logxLOG(__VA_ARGS__)[::logx::tags::info]
#define logxWARN(...)		logxLOG(__VA_ARGS__)[::logx::tags::warning]
#define logxERROR(...)		logxLOG(__VA_ARGS__)[::logx::tags::error]
#define logxFATAL(...)		logxLOG(__VA_ARGS__)[::logx::tags::fatal]



#endif

#endif
