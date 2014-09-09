#pragma once
#ifndef _LOGX_CORE_HPP
#define _LOGX_CORE_HPP


#include <typeinfo>
#include <type_traits>
#include <functional>
#include <memory>
#include <list>
#include "config.hpp"
#include "details/message.hpp"


namespace logx {
	class sink_message;
	typedef std::function<void(const sink_message&)> sink;

	class wrapped_sink : public std::enable_shared_from_this<wrapped_sink>
	{
	public:
		virtual ~wrapped_sink() {}
		virtual sink wrap() = 0;
	};

	typedef std::shared_ptr<wrapped_sink> wrapped_sink_ptr;

	namespace details {


		template<typename Dummy = void>
		class log_thread;
		
		template<typename Dummy = void>
		class log_core;

		template<>
		class log_core<void> : public std::enable_shared_from_this<log_core<void>>
		{
			template<typename _Dummy>
			friend class log_thread;
		public:
			virtual ~log_core() {}

			virtual void init() = 0;
			virtual void add_sink(sink _sink) = 0;
			virtual void add_wrapped_sink(wrapped_sink_ptr _sink) = 0;


			virtual bool remove_default_tag(const std::type_info& _ty) = 0;

			template<typename Tag, typename std::enable_if<std::is_base_of<tag, Tag>::value>::type* = nullptr>
			std::shared_ptr<tag> add_default_tag(Tag&& _tag)
			{
				return _add_default_tag(typeid(Tag), std::make_shared<Tag>(std::forward<Tag>(_tag)));
			}



			static log_core& get_core();
			static void set_core(log_core& other);

			template<typename _MsgTy, typename... _Args>
			void add_log_message(_MsgTy&& _msg, _Args&&... _args)
			{
				typedef message<_MsgTy, _Args...> msg_type;
				_msg_creator* creator = _get_creator(sizeof(msg_type));
				msg_type* msg = new (creator->msg_mem) msg_type(std::forward<_MsgTy>(_msg), std::forward<_Args>(_args)...);
				creator->done(msg);
			}
		protected:
			struct _msg_creator
			{
				virtual ~_msg_creator() {}
				void* msg_mem;
				virtual void done(message_base* msg) = 0;
			};

			virtual _msg_creator* _get_creator(std::size_t _msg_size) = 0;
			virtual std::shared_ptr<tag> _add_default_tag(const std::type_info& _ty, const std::shared_ptr<tag>& _tag) = 0;

		private:
			static std::shared_ptr<log_core> GLogCore;
		};
	}

	typedef details::log_core<> core;
}






#endif
