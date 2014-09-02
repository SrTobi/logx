#pragma once
#ifndef _LOGX_ADDITIONAL_STREAMS_HPP
#define _LOGX_ADDITIONAL_STREAMS_HPP


#include <ostream>
#include <chrono>
#include "details/char_conv.hpp"


namespace logx {

	namespace additional_streams {


		/****************************************************************************************************************************/
		// chrono streams
		template<typename Ch, typename Ty>
		std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& _stream, const std::chrono::duration<Ty, std::nano>& _dur)
		{
			_stream << _dur.count() << LOGX_LITERAL(Ch, "ns");

			return _stream;
		}

		template<typename Ch, typename Ty>
		std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& _stream, const std::chrono::duration<Ty, std::micro>& _dur)
		{
			_stream << _dur.count() << LOGX_LITERAL(Ch, "micro");

			return _stream;
		}

		template<typename Ch, typename Ty>
		std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& _stream, const std::chrono::duration<Ty, std::milli>& _dur)
		{
			_stream << _dur.count() << LOGX_LITERAL(Ch, "ms");

			return _stream;
		}
		
		template<typename Ch, typename Ty>
		std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& _stream, const std::chrono::duration<Ty, std::ratio<1>>& _dur)
		{
			_stream << _dur.count() << LOGX_LITERAL(Ch, "sec");

			return _stream;
		}

		template<typename Ch, typename Ty>
		std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& _stream, const std::chrono::duration<Ty, std::ratio<60>>& _dur)
		{
			_stream << _dur.count() << LOGX_LITERAL(Ch, "min");

			return _stream;
		}

		template<typename Ch, typename Ty>
		std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& _stream, const std::chrono::duration<Ty, std::ratio<60 * 60>>& _dur)
		{
			_stream << _dur.count() << LOGX_LITERAL(Ch, "h");

			return _stream;
		}

		template<typename Ch, typename Ty, typename Period>
		std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& _stream, const std::chrono::duration<Ty, Period>& _dur)
		{
			typedef std::ratio<60> minute_ratio;
			typedef std::ratio<60 * 60> hour_ratio;
			const auto num = Period::num;
			const auto den= Period::den;

			if (num >= den)
			{
				const auto div = num / den;

				if (div >= hour_ratio::num)
				{
					_stream << std::chrono::duration_cast<std::chrono::duration<Ty, hour_ratio>>(_dur);
				}
				else if (div >= minute_ratio::num)
				{
					_stream << std::chrono::duration_cast<std::chrono::duration<Ty, minute_ratio>>(_dur);
				}
				else
				{
					_stream << std::chrono::duration_cast<std::chrono::duration<Ty, std::ratio<1>>>(_dur);
				}
			}
			else
			{
				const auto div = den / num;

				if (div >= std::nano::den)
	 			{
					_stream << std::chrono::duration_cast<std::chrono::duration<Ty, std::nano>>(_dur);
				}
				else if (div >= std::micro::den)
				{
					_stream << std::chrono::duration_cast<std::chrono::duration<Ty, std::micro>>(_dur);
				}
				else
				{
					_stream << std::chrono::duration_cast<std::chrono::duration<Ty, std::milli>>(_dur);
				}
			}

			return _stream;
		}


		/****************************************************************************************************************************/
		// string streams
		static std::wostream& operator << (std::wostream& _stream, const std::string& str)
		{
			_stream << details::convert_wstring(str);
			return _stream;
		}

		static std::ostream& operator << (std::ostream& _stream, const std::wstring& str)
		{
			_stream << details::convert_string(str);
			return _stream;
		}

		static std::wostream& operator << (std::wostream& _stream, const char* str)
		{
			_stream << details::convert_wstring(str);
			return _stream;
		}

		static std::ostream& operator << (std::ostream& _stream, const wchar_t* str)
		{
			_stream << details::convert_string(str);
			return _stream;
		}
	}
}





#endif