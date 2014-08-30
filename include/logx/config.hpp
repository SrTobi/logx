#pragma once
#ifndef _LOGX_CONFIG_HPP
#define _LOGX_CONFIG_HPP


// #define LOGXCFG_FORCE_DEBUG_BUILD
// #define LOGXCFG_FORCE_DEBUG_LOG
// #define LOGXCFG_FORCE_RELEASE_LOG
// #define LOGXCFG_SYNC
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


#define _LOGXW(_text) L##_text
#define LOGXW(_text) _LOGXW(_text)

namespace logx {

#ifdef LOGXCFG_USE_WCHAR
	typedef wchar_t char_type;
	typedef std::wstring string;
#	define LOGXTXT(_text) _LOGXW(_text)
#else
	typedef char char_type;
	typedef std::string string;
#	define LOGXTXT(_text) _text
#endif

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