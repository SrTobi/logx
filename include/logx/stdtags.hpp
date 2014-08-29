#pragma once
#ifndef _LOGX_STDTAGS_HPP
#define _LOGX_STDTAGS_HPP

#include <chrono>
#include "tag.hpp"


#ifndef __FUNCTION__
#	define __FUNCTION__ "<unknown>"
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


		/**************************************************************************************************/
		// tag: source
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


		/**************************************************************************************************/
		// tag: cat
		logxTAG(cat,
			(
				std::get<0>(_args)
			),
			std::wstring
		)

		/**************************************************************************************************/
		// tag: log_level
		namespace details {
			enum class _log_level : int8_t
			{
				TRACE = 0,
				DEBUG,
				DETAIL,
				INFO,
				WARNING,
				ERROR,
				FATAL,
				_LOGLVL_COUNT
			};

			static const wchar_t* LogLevelNames[int8_t(_log_level::_LOGLVL_COUNT)] =
				{
					LOGXW("TRACE"),
					LOGXW("DEBUG"),
					LOGXW("DETAIL"),
					LOGXW("INFO"),
					LOGXW("WARN"),
					LOGXW("ERR"),
					LOGXW("FATAL")
				};
			
			template <typename E>
			auto to_underlying(E e) -> typename std::underlying_type<E>::type
			{
				return static_cast<typename std::underlying_type<E>::type>(e);
			}
		}

		logxTAG(log_level,
			(
				details::LogLevelNames[details::to_underlying(std::get<0>(_args))]
			),
			details::_log_level
		)

		static log_level trace(details::_log_level::TRACE);
		static log_level debug(details::_log_level::DEBUG);
		static log_level info(details::_log_level::INFO);
		static log_level detail(details::_log_level::INFO);
		static log_level warning(details::_log_level::WARNING);
		static log_level error(details::_log_level::ERROR);
		static log_level fatal(details::_log_level::FATAL);

		/**************************************************************************************************/
		// tag: time
		class time : public tag
		{
		public:
			typedef std::chrono::system_clock clock_type;
			typedef clock_type::time_point time_point;

		public:
			time()
				: mTime(std::chrono::system_clock::now())
			{
			}

			time(time_point _time)
				: mTime(_time)
			{
			}

			virtual std::wstring name() const override
			{
				return L"time";
			}

			virtual std::wstring value() const override
			{
				return std::to_wstring(mTime.time_since_epoch().count());
			}

		private:
			const time_point mTime;
		};

	}
}



#endif