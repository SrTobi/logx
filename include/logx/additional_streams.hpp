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
			_stream << _dur.count() << LOGX_LITERAL(Ch, "mics");

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

		template<typename Ch, typename Ty, typename Period, typename std::enable_if<std::is_integral<Ty>::value>::type* = nullptr>
		std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& _stream, const std::chrono::duration<Ty, Period>& _dur)
		{
			_stream << std::chrono::duration_cast<std::chrono::milliseconds>(_dur);

			return _stream;
		}

		template<typename Ch, typename Ty, typename Period, typename std::enable_if<std::is_floating_point<Ty>::value>::type* = nullptr>
		std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& _stream, const std::chrono::duration<Ty, Period>& _dur)
		{
			_stream << std::chrono::duration_cast<std::chrono::duration<Ty, std::ratio<1>>(_dur);

			return _stream;
		}
	}
}





#endif