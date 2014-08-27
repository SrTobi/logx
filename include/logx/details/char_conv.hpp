#pragma once
#ifndef _LOGX_UTF8_HPP
#define _LOGX_UTF8_HPP


#include <string>
#include <type_traits>
#include <codecvt>


namespace logx {

	namespace details{

		static std::wstring to_wstring(std::wstring _str)
		{
			return _str;
		}

		static std::wstring to_wstring(const std::string& _str)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.from_bytes(_str.c_str());
		}


		namespace streaming_fallback
		{
			struct anything { template<typename X> anything(const X&){} };
			static std::wostream& operator <<(std::wostream& stream, const anything&)
			{
				stream << L"[unknown]";
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