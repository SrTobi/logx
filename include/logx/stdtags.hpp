#pragma once
#ifndef _LOGX_STDTAGS_HPP
#define _LOGX_STDTAGS_HPP

#include "tag.hpp"


#ifndef __FUNCTION__
#	define __FUNCTION__ ""
#endif

namespace logx {
	namespace tags {

		namespace details {
			static std::wstring trim_file_path(const wchar_t* _file)
			{
				const wchar_t* it = _file;
				while (*it)
				{
					if(*it == L'/' || *it == L'\\')
						_file = it + 1;

					++it;
				}
				return std::wstring(_file);
			}

		}

		logxTAG(source,
			(
			details::trim_file_path(std::get<0>(_args)) + LOGXW(" in ")
			+ std::get<1>(_args) +LOGXW("(...) at ")
			+ std::to_wstring(std::get<2>(_args))

			),
			const wchar_t* /* file */,
			const wchar_t* /* function */,
			unsigned int /* line */
			)
#define logxSOURCE ::logx::tags::source(LOGXW(__FILE__), LOGXW(__FUNCTION__), __LINE__)




	}
}



#endif