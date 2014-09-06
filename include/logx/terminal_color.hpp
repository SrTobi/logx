#pragma once
#ifndef _LOGX_TERMINAL_COLOR_HPP
#define _LOGX_TERMINAL_COLOR_HPP


#include <ostream>
#include "config.hpp"

namespace logx {

	namespace details {


#ifdef WIN32
		template<typename Dummy = void>
		class terminal_color_impl
		{
		public:
			enum Colors
			{
				Empty,
				Black,
				White,
				Red,
				Green,
				Blue,
				Yellow
			};
		public:
			terminal_color_impl(Colors _fcolor, Colors _bcolor = Empty)
				: mConsoleHandle(nullptr)
				, mFColor(_fcolor)
				, mBColor(_bcolor)
			{
			}

			~terminal_color_impl()
			{
			}

			static bool supported()
			{
				return true;
			}

			template<typename Ch>
			void bind(std::basic_ostream<Ch>& _stream)
			{
				mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO info;
				if (GetConsoleScreenBufferInfo(mConsoleHandle, &info))
				{
					mOldColor = info.wAttributes;
				}

				SetConsoleTextAttribute(mConsoleHandle, _resolve_fcolor(mFColor) | _resolve_bcolor(mBColor));
			}

			template<typename Ch>
			void unbind(std::basic_ostream<Ch>& _stream)
			{
				if (mConsoleHandle)
				{
					SetConsoleTextAttribute(mConsoleHandle, mOldColor);
				}
			}

		private:
			static WORD _resolve_fcolor(Colors _col)
			{
				switch (_col)
				{
				case Black:
					return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
				case White :
					return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
				case Red:
					return FOREGROUND_RED | FOREGROUND_INTENSITY;
				case Green:
					return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				case Blue:
					return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				case Yellow:
					return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				case Empty:
				default:
					return 0;
				}
			}

			static WORD _resolve_bcolor(Colors _col)
			{
				switch (_col)
				{
				case Black:
					return BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
				case White :
					return BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
				case Red:
					return BACKGROUND_RED | BACKGROUND_INTENSITY;
				case Green:
					return BACKGROUND_GREEN | BACKGROUND_INTENSITY;
				case Blue:
					return BACKGROUND_BLUE | BACKGROUND_INTENSITY;
				case Yellow:
					return BACKGROUND_RED | BACKGROUND_GREEN | FOREGROUND_INTENSITY;
				case Empty:
				default:
					return 0;
				}
			}

		private:
			HANDLE mConsoleHandle;
			Colors mFColor;
			Colors mBColor;
			WORD mOldColor;
		};

#endif


		template<typename Ch>
		class color_stream : public std::basic_ostream<Ch>
		{
		public:
			color_stream(color_stream&& _old)
				: std::basic_ostream<Ch>(_old.mStream->rdbuf())
				, mBound(_old.mBound)
				, mStream(_old.mStream)
				, mColor(std::move(_old.mColor))
			{
				_old.mBound = false;
			}

			color_stream(std::basic_ostream<Ch>& _stream, terminal_color_impl<> color)
				: std::basic_ostream<Ch>(_stream.rdbuf())
				, mColor(std::move(color))
				, mBound(true)
				, mStream(_stream)
			{
				mColor.bind(mStream);
			}

			~color_stream()
			{
				if (mBound)
				{
					mColor.unbind(mStream);
					mBound = false;
				}
			}
			
		private:
			terminal_color_impl<> mColor;
			bool mBound;
			std::basic_ostream<Ch>& mStream;
		};
	}

	typedef details::terminal_color_impl<> terminal_color;
}


namespace std {
	template<typename Ch>
	logx::details::color_stream<Ch> operator <<(std::basic_ostream<Ch>& _stream, logx::terminal_color _color)
	{
		return logx::details::color_stream<Ch>(_stream, _color);
	}
}

#endif