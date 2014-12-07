#pragma once
#ifndef _LOGX_STDTAGS_HPP
#define _LOGX_STDTAGS_HPP

#include <thread>
#include <sstream>
#include "details/char_conv.hpp"
#include "tag.hpp"


#ifndef __FUNCTION__
#	define __FUNCTION__ ""
#endif

namespace logx {
	namespace tags {

		namespace details {
			static string trim_file_path(const char* _file)
			{
				const char* it = _file;
				while (*it)
				{
					if(*it == '/' || *it == '\\')
						_file = it + 1;

					++it;
				}
				return string(_file);
			}

		}


		/**************************************************************************************************/
		// tag: source
		logxTAG(source,
			(
			details::trim_file_path(std::get<0>(_args)) + " in "
			+ std::get<1>(_args) + "(...) at "
			+ logx::details::std_to_string(std::get<2>(_args))

			),
			const char* /* file */,
			const char* /* function */,
			unsigned int /* line */
		)
#define logxSOURCE ::logx::tags::source(__FILE__, __FUNCTION__, __LINE__)


		/**************************************************************************************************/
		// tag: cat
		logxTAG(cat,
			(
				std::get<0>(_args)
			),
			const string
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
				ERR,
				FATAL,
				_LOGLVL_COUNT
			};

			static const char* LogLevelNames[int8_t(_log_level::_LOGLVL_COUNT)] =
				{
					"TRACE",
					"DEBUG",
					"DETAIL",
					"INFO",
					"WARN",
					"ERR",
					"FATAL"
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
		static log_level detail(details::_log_level::DETAIL);
		static log_level info(details::_log_level::INFO);
		static log_level warning(details::_log_level::WARNING);
		static log_level error(details::_log_level::ERR);
		static log_level fatal(details::_log_level::FATAL);
		

		/**************************************************************************************************/
		// tag: thread
		class thread : public tag
		{
		public:
			thread()
				: mThreadId(std::this_thread::get_id())
			{
			}

			thread(std::thread::id _id)
				: mThreadId(_id)
			{
			}

			virtual string name() const override
			{
				return "thread";
			}

			virtual string value() const override
			{
				std::ostringstream stream;
				stream << mThreadId;
				return stream.str();
			}
		private:
			const std::thread::id mThreadId;
		};


		/**************************************************************************************************/
		// tag: job
		logxTAG(job,
			(
			 std::get<0>(_args)
			),
			const string /* job */
		);
	}
}



#endif