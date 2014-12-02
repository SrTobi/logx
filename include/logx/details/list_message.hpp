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


		template<typename T>
		struct uncopyable_element_wrapper
		{
			const T& value;
		};

		template<typename T>
		class list_message_element_wrapper
		{
		public:
			template<typename Arg>
			list_message_element_wrapper(Arg&& _arg)
				: mValue(std::forward<Arg>(_arg))
			{}

			template<typename Base>
			void push_if_based(std::vector<const Base*>& _target) const
			{
				_push_if_based(_target, &mValue);
			}

			void write_to_msg(std::ostringstream& os) const
			{
				_add_to_msg(os, mValue);
			}

		private:
			template<typename Base>
			static void _push_if_based(std::vector<const Base*>& _target, const Base* _arg)
			{
				_target.push_back(_arg);
			}

			template<typename Base>
			static void _push_if_based(std::vector<const Base*>& _target, const void* _arg)
			{
			}

			static void _add_to_msg(std::ostringstream& _os, const tag& _val)
			{
			}

			template<typename _T>
			static typename std::enable_if<!std::is_base_of<tag, _T>::value>::type
					_add_to_msg(std::ostringstream& _os, const _T& _val)
			{
				using namespace additional_streams;
				_os << _val;
			}

		private:
			T mValue;
		};

		template<typename T>
		class list_message_element_wrapper<uncopyable_element_wrapper<T> >
		{
		public:
			list_message_element_wrapper(const uncopyable_element_wrapper<T>& _arg)
			{
				using namespace additional_streams;
				std::ostringstream os;
				os << _arg.value;
				mMsgValue = os.str();
			}

			template<typename Base>
			void push_if_based(std::vector<const Base*>& _target) const
			{
			}

			void write_to_msg(std::ostringstream& os) const
			{
				os << mMsgValue;
			}
			
		private:
			std::string mMsgValue;
		};




		template<std::size_t _ACount, typename... _Args>
		class list_message : public message_base
		{
			static_assert(_ACount <= sizeof...(_Args), "_ACount can not be greater than _Args");
		public:
			template<typename... _AArgs>
			list_message(_AArgs&&... _args)
				: mArguments(std::forward<_AArgs>(_args)...)
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
				std::get<I>(mArguments).write_to_msg(_os);
				_build_msg<I + 1>(_os);
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
				std::get<I>(mArguments).push_if_based(_args);
				_build<I + 1>(_args);
			}


		private:
			std::tuple<list_message_element_wrapper<typename std::decay<_Args>::type>...> mArguments;
		};


	}
}


#endif
