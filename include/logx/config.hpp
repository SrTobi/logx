#pragma once
#ifndef _LOGX_CONFIG_HPP
#define _LOGX_CONFIG_HPP


// #define LOGXCFG_FORCE_DEBUG_BUILD
// #define LOGXCFG_FORCE_DEBUG_LOG
// #define LOGXCFG_FORCE_RELEASE_LOG
// #define LOGXCFG_SYNC


#if !defined(NDEBUG) || defined(LOGXCFG_FORCE_DEBUG_BUILD)
#	define LOGX_DEBUG
#endif



#if defined(LOGX_DEBUG) || defined(LOGXCFG_FORCE_DEBUG_LOG)
#	ifndef LOGCFG_FORCE_RELEASE_LOG
#		define LOGX_DEBUG_LOG
#	else
#		ifdef LOGXCFG_FORCE_DEBUG_LOG
#			error "LOGCFG_FORCE_RELEASE_LOG and LOGXCFG_FORCE_DEBUG_LOG must not be defined at the same time!"
#		endif
#	endif

#endif


#ifdef LOGXCFG_FORCE_WCHAR
#	define _LOGXW(_text) L##_text
#	define LOGXW(_text) _LOGXW(_text)
#else
#	define LOGXW(_text) _text
#endif



#endif