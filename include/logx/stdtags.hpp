#pragma once
#ifndef _LOGX_STDTAGS_HPP
#define _LOGX_STDTAGS_HPP

#include <thread>
#include <sstream>
#include "config.hpp"
#include "details/char_conv.hpp"
#include "tag.hpp"


#ifndef __FUNCTION__
#	define __FUNCTION__ "[unknown]"
#	define _LOGX_NO_FUNCTION
#	define _LOGX_PRINT_IF_FUNCTION(_v)
#else
#	define _LOGX_PRINT_IF_FUNCTION(_v)	_v
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
		logxTAG_BEGIN(source,
				(
				filename() + " in "
				_LOGX_PRINT_IF_FUNCTION(+ function() + "(...) at ")
				+ logx::details::std_to_string(line())

				),
				const char* /* file */,
				const char* /* function */,
				size_t		/* line */
			)

			inline string filename() const { return details::trim_file_path(arg<0>()); }
			inline const char* filepath() const { return arg<0>(); }
			inline const char* function() const { return arg<1>(); }
			inline size_t line() const { return arg<2>(); }

		logxTAG_END(source)
#define logxSOURCE ::logx::tags::source(__FILE__, __FUNCTION__, __LINE__)


		/**************************************************************************************************/
		// tag: cat
		logxTAG_BEGIN(cat,
				(
					arg<0>()
				),
				const string
			)

			inline const string& category() const { return arg<0>(); }

		logxTAG_END(cat);

		/**************************************************************************************************/
		// tag: log_level
		enum class log_levels : int8_t
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

		namespace details {
			static const char* LogLevelNames[int8_t(log_levels::_LOGLVL_COUNT)] =
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

		logxTAG_BEGIN(log_level,
				(
					details::LogLevelNames[details::to_underlying(arg<0>())]
				),
				log_levels
			)

			inline log_levels level() const { return arg<0>(); }
		logxTAG_END(log_level)

		static log_level trace(log_levels::TRACE);
		static log_level debug(log_levels::DEBUG);
		static log_level detail(log_levels::DETAIL);
		static log_level info(log_levels::INFO);
		static log_level warning(log_levels::WARNING);
		static log_level error(log_levels::ERR);
		static log_level fatal(log_levels::FATAL);
		

		/**************************************************************************************************/
		// tag: thread

		class LOGX_EXPORT std::thread::id;
		class LOGX_EXPORT thread : public tag
		{
		public:
			thread();
			thread(std::thread::id _id);

			virtual string name() const override;
			virtual string value() const override;

			inline std::thread::id id() const { return mThreadId; }
		private:
			const std::thread::id mThreadId;
		};
		logxTAG_PACK_OPERATOR(thread);


		/**************************************************************************************************/
		// tag: job
		logxTAG_BEGIN(job,
				(
					arg<0>()
				),
				const string /* job */
			)

			inline const string& jobname() const { return arg<0>(); }

		logxTAG_END(job);

		/**************************************************************************************************/
		// tag: time
		class LOGX_EXPORT time : public tag
		{
		public:
			time();

			virtual string name() const override;
			virtual string value() const override;

		private:
			const int64_t mTime;
		};
		logxTAG_PACK_OPERATOR(time);
	}
}



#endif