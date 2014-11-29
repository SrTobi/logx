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
#include <logx/logx_api.hpp>


namespace logx {
	class sink_message;
	typedef std::function<void(const sink_message&)> sink;

	class log_thread;

	class LOGX_EXPORT core : public std::enable_shared_from_this<core>
	{
		friend class log_thread;
	public:
		virtual ~core() {}

		virtual void init() = 0;
		virtual void add_sink(sink _sink) = 0;


		virtual bool remove_default_tag(const std::type_info& _ty) = 0;

		template<typename Tag, typename std::enable_if<std::is_base_of<tag, Tag>::value>::type* = nullptr>
		std::shared_ptr<tag> add_default_tag(Tag&& _tag)
		{
			return _add_default_tag(typeid(Tag), std::make_shared<Tag>(std::forward<Tag>(_tag)));
		}



		static core& get_core();
		static void set_core(core& other);

		template<typename _MsgTy, typename... _Args>
		void add_log_message(_MsgTy&& _msg, _Args&&... _args)
		{
			typedef details::message<_MsgTy, _Args...> msg_type;
			_msg_creator* creator = _get_creator(sizeof(msg_type));
			msg_type* msg = new (creator->msg_mem) msg_type(std::forward<_MsgTy>(_msg), std::forward<_Args>(_args)...);
			creator->done(msg);
		}
	protected:
		struct _msg_creator
		{
			virtual ~_msg_creator() {}
			void* msg_mem;
			virtual void done(details::message_base* msg) = 0;
		};

		virtual _msg_creator* _get_creator(std::size_t _msg_size) = 0;
		virtual std::shared_ptr<tag> _add_default_tag(const std::type_info& _ty, const std::shared_ptr<tag>& _tag) = 0;

	private:
		static std::shared_ptr<core> GLogCore;
	};
}






#endif
