#pragma once
#ifndef _LOGX_MESSAGE_HPP
#define _LOGX_MESSAGE_HPP


#include <tuple>
#include <string>
#include <sstream>
#include <typeinfo>
#include <type_traits>
#include <vector>
#include "char_conv.hpp"

namespace logx {
	struct sink_base;

	namespace details {
		template<typename _Ty>
		class specific_message_arg;
	}

	class message_arg
	{
		template<typename _Ty>
		friend class details::specific_message_arg;
	public:
		virtual const std::type_info& type() const = 0;
		virtual std::wstring to_string() const = 0;
		template<typename _Ty>
		_Ty* as()
		{
			return dynamic_cast<specific_message_arg<_Ty>*>(this)->ptr();
		}

		template<typename _Ty>
		const _Ty* as() const
		{
			return dynamic_cast<const specific_message_arg<_Ty>*>(this)->ptr();
		}

	private:
		message_arg()
		{}
	};
	
	
	namespace details {

		class message_base
		{
		public:
			virtual std::wstring msg() const = 0;
			virtual int arg_count() const = 0;
			virtual std::vector<const message_arg*> args() const = 0;
		};

		template<typename _Ty>
		class specific_message_arg : public message_arg
		{
		public:
			specific_message_arg(_Ty&& _val)
				: mValue(std::forward<_Ty>(_val))
			{}

			virtual const std::type_info& type() const override
			{
				return typeid(_Ty);
			}

			virtual std::wstring to_string() const override
			{
				using namespace streaming_fallback;
				std::wostringstream ss;
				ss << std::boolalpha << mValue;
				return ss.str();
			}

			auto ptr() -> typename std::remove_reference < _Ty >::type*
			{
				return &mValue;
			}

			auto ptr() const -> typename std::remove_reference < _Ty >::type*
			{
				return &mValue;
			}

		private:
			_Ty mValue;
		};

		template<typename _MsgTy, typename... _Args>
		class message : public message_base
		{
		public:

			message(_MsgTy&& _msg, _Args&&... _args)
				: mMessage(std::forward<_MsgTy>(_msg))
				, mArguments(std::forward<_Args>(_args)...)
			{
			}

			virtual std::wstring msg() const override
			{
				return to_wstring(mMessage);
			}

			int arg_count() const override
			{
				return sizeof...(_Args);
			}

			virtual std::vector<const message_arg*> args() const
			{
				std::vector<const message_arg*> _result;
				_result.reserve(sizeof...(_Args));
				_build_args(_result);
				return _result;
			}
		private:
			template<std::size_t I = 0>
				typename std::enable_if<I == sizeof...(_Args)>::type
				_build_args(std::vector<const message_arg*>& _args) const
			{
			}

			template<std::size_t I = 0>
				typename std::enable_if<(I < sizeof...(_Args))>::type
				_build_args(std::vector<const message_arg*>& _args) const
			{
				_args.push_back(&std::get<I>(mArguments));
				_build_args<I + 1>(_args);
			}

		private:
			_MsgTy mMessage;
			std::tuple<specific_message_arg<_Args>...> mArguments;
		};






	}
}



#endif