#pragma once
#ifndef _LOGX_CORE_HPP
#define _LOGX_CORE_HPP


#include <type_traits>
#include <functional>
#include <memory>
#include <list>
#include "config.hpp"
#include "details/message.hpp"


namespace logx {
	class sink_message;
	typedef std::function<void(const sink_message&)> sink;

	namespace details {


		template<typename Dummy = void>
		class log_thread;
		
		template<typename Dummy = void>
		class log_core;

		template<>
		class log_core<void> : private std::enable_shared_from_this<log_core<void>>
		{
			template<typename _Dummy>
			friend class log_thread;
		public:
			virtual ~log_core() {}

			virtual void init() = 0;
			virtual void add_sink(sink _sink) = 0;

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


		private:
			static std::shared_ptr<log_core> GLogCore;
		};
	}
	typedef details::log_core<> core;
}






#endif
