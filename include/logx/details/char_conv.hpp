#pragma once
#ifndef _LOGX_UTF8_HPP
#define _LOGX_UTF8_HPP


#include <string>
#include <type_traits>
#include "../config.hpp"

#if _LOGX_FEATURE_STD_CODECVT != _LOGX_FEATURE_HAS_NOT
#	include <codecvt>
#	define _LOGX_STD_CODECVT
#endif


namespace logx {

	namespace details{


		static std::string convert_string(const std::wstring& _str)
		{

#ifdef _LOGX_STD_CODECVT
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.to_bytes(_str.c_str());
#else
			string result;

			for(auto c : _str)
				result += (c >= 128)? '?' : c;

			return result;
#endif
		}


		static std::wstring convert_wstring(const std::string& _str)
		{
#ifdef _LOGX_STD_CODECVT
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.from_bytes(_str.c_str());
#else
			return std::wstring(_str.begin(), _str.end());
#endif
		}



#ifdef LOGXCFG_USE_WCHAR
		static string to_string(std::wstring _str)
		{
			return _str;
		}

		static string to_string(const std::string& _str)
		{
			return convert_wstring(_str);
		}

		template<typename T>
		string std_to_string(const T& _var)
		{
			return std::to_wstring(_var);
		}

#else

		static string to_string(std::string _str)
		{
			return _str;
		}

		static string to_string(const std::wstring& _str)
		{
			return convert_string(_str);
		}

		template<typename T>
		string std_to_string(const T& _var)
		{
			return std::to_string(_var);
		}

#endif

		namespace streaming_fallback
		{
			struct anything { template<typename X> anything(const X&){} };

			template<typename Ch>
			std::basic_ostream<Ch>& operator <<(std::basic_ostream<Ch>& stream, anything)
			{
				stream << LOGX_LITERAL(Ch, "[unknown]");
				return stream;
			}

		}

	}


	/*
	class utf8_string
	{
	public:
		utf8_string()
		{
		}

		utf8_string(std::string _content)
			: mBuffer(std::move(_content))
		{
		}

		std::size_t size() const
		{
			return mBuffer.size();
		}

		const char* data() const
		{
			return mBuffer.data();
		}

		std::wstring wstr() const
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		    return converter.from_bytes(mBuffer);
		}

	private:
		std::string mBuffer;
	};
	*/


}





#endif
