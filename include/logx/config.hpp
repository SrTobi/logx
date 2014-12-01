#pragma once
#ifndef _LOGX_CONFIG_HPP
#define _LOGX_CONFIG_HPP


// #define LOGXCFG_FORCE_DEBUG_BUILD
// #define LOGXCFG_FORCE_DEBUG_LOG
// #define LOGXCFG_FORCE_RELEASE_LOG
#define LOGXCFG_SYNC
// #define LOGXCFG_USE_WCHAR


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

#define _LOGX_FEATURE_HAS_NOT		0
#define _LOGX_FEATURE_HAS			1
#define _LOGX_FEATURE_HAS_MAYBE		2

#ifdef __GNUC__
#	define _LOGX_FEATURE_STD_CODECVT _LOGX_FEATURE_HAS_NOT
#else
#	ifdef _MSC_VER
#		define _LOGX_FEATURE_STD_CODECVT _LOGX_FEATURE_HAS
#	else
#		define _LOGX_FEATURE_STD_CODECVT _LOGX_FEATURE_HAS_MAYBE
#	endif
#endif






#define _LOGXW(_text) L##_text
#define LOGXW(_text) _LOGXW(_text)

namespace logx {

	typedef std::string string;

#define LOGX_LITERAL(T,x) ::logx::details::string_traits<T>::choose(x, L##x)

	namespace details {

		template<typename T>
		struct string_traits
		{
			typedef char char_type;
			static const char * choose(const char * narrow, const wchar_t * wide) { return narrow; }
			static char choose(char narrow, wchar_t wide) { return narrow; }
		};

		template<>
		struct string_traits <wchar_t>
		{
			typedef wchar_t char_type;
			static const wchar_t * choose(const char * narrow, const wchar_t * wide) { return wide; }
			static wchar_t choose(char narrow, wchar_t wide) { return wide; }
		};

	}

}


#endif
