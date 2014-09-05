#pragma once
#ifndef _LOGX_MESSAGE_HPP
#define _LOGX_MESSAGE_HPP


#include <tuple>
#include <string>
#include <sstream>
#include <typeinfo>
#include <type_traits>
#include <vector>
#include "../tag.hpp"
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
		virtual string to_string() const = 0;
		template<typename _Ty>
		_Ty* as()
		{
			return dynamic_cast<details::specific_message_arg<_Ty>*>(this)->ptr();
		}

		template<typename _Ty>
		const _Ty* as() const
		{
			return dynamic_cast<const details::specific_message_arg<_Ty>*>(this)->ptr();
		}

		virtual const tag* as_tag() const = 0;

	private:
		message_arg()
		{}
	};
	
	
	namespace details {

		template<typename Target, typename Source,
			typename std::enable_if<std::is_base_of<Target, Source>::value>::type* = nullptr>
		Target* cast_up(Source* _src)
		{
			return _src;
		}

		template<typename Target, typename Source,
			typename std::enable_if<!std::is_base_of<Target, Source>::value>::type* = nullptr>
		Target* cast_up(Source* _src)
		{
			return nullptr;
		}

		class message_base
		{
		public:
			virtual string msg() const = 0;
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

			virtual string to_string() const override
			{
				using namespace streaming_fallback;
				std::basic_ostringstream<char_type> ss;
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

			virtual const tag* as_tag() const override
			{
				return cast_up<const tag>(&mValue);
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

			virtual string msg() const override
			{
				return to_string(mMessage);
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
