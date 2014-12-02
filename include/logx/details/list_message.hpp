#pragma once
#ifndef _LOGX_LIST_MESSAGE_HPP
#define _LOGX_LIST_MESSAGE_HPP



#include <tuple>
#include <sstream>
#include <typeinfo>
#include <type_traits>

#include "base_message.hpp"
#include "char_conv.hpp"
#include "../additional_streams.hpp"


namespace logx {
	namespace details {
		template<std::size_t _ACount, typename... _Args>
		class list_message : public message_base
		{
			static_assert(_ACount <= sizeof...(_Args), "_ACount can not be greater than _Args");
		public:
			list_message(_Args&&... _args)
				: mArguments(std::forward<_Args>(_args)...)
			{
			}

			virtual string msg() const override
			{
				std::ostringstream os;
				os << std::boolalpha;
				_build_msg(os);

				return os.str();
			}

			virtual std::vector<const tag*> tags() const override
			{
				std::vector<const tag*> _result;
				_result.reserve(sizeof...(_Args));
				_build(_result);
				return _result;
			}

			virtual std::vector<const annotation*> annotations() const override
			{
				std::vector<const annotation*> _result;
				_result.reserve(sizeof...(_Args));
				_build(_result);
				return _result;
			}
		private:

			/************************************** build message text **************************************/
			template<std::size_t I = 0>
				typename std::enable_if<I >= _ACount>::type
				_build_msg(std::ostringstream& _os) const
			{
			}

			template<std::size_t I = 0>
				typename std::enable_if<(I < _ACount)>::type
				_build_msg(std::ostringstream& _os) const
			{
				_add_to_msg(_os, std::get<I>(mArguments));
				_build_msg<I + 1>(_os);
			}

			static void _add_to_msg(std::ostringstream& _os, const tag& _val)
			{
			}

			template<typename T>
			static typename std::enable_if<!std::is_base_of<tag, T>::value>::type
					_add_to_msg(std::ostringstream& _os, const T& _val)
			{
				using namespace additional_streams;
				_os << _val;
			}

			/************************************** build tag/annotation vector **************************************/
			template<std::size_t I = 0, class Base>
				typename std::enable_if<I >= sizeof...(_Args)>::type
				_build(std::vector<const Base*>& _args) const
			{
			}

			template<std::size_t I = 0, class Base>
				typename std::enable_if<(I < sizeof...(_Args))>::type
				_build(std::vector<const Base*>& _args) const
			{
				_push_if(_args, &std::get<I>(mArguments));
				_build<I + 1>(_args);
			}

			template<typename Base>
			static void _push_if(std::vector<const Base*>& _target, const Base* _arg)
			{
				_target.push_back(_arg);
			}

			template<typename Base>
			static void _push_if(std::vector<const Base*>& _target, const void* _arg)
			{
			}

		private:
			std::tuple<_Args...> mArguments;
		};


	}
}


#endif