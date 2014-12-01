#pragma once
#ifndef _LOGX_CORE_HPP
#define _LOGX_CORE_HPP


#include <typeinfo>
#include <type_traits>
#include <functional>
#include <memory>
#include <list>
#include "config.hpp"
#include "details/base_message.hpp"
#include <logx/logx_api.hpp>


namespace logx {
	class sink_message;
	typedef std::function<void(const sink_message&)> sink;

	class log_thread;

	class LOGX_EXPORT core
	{
		friend class log_thread;
	public:
		virtual ~core();
		virtual void add_sink(sink _sink) = 0;
		virtual void remove_all_sinks() = 0;

		template<typename Tag>
		std::shared_ptr<tag> add_default_tag(Tag&& _tag)
		{
			static_assert(std::is_base_of<tag, Tag>::value, "Tag must inherit 'tag'!");
			return _add_default_tag(typeid(Tag), std::make_shared<Tag>(std::forward<Tag>(_tag)));
		}

		virtual bool remove_default_tag(const std::type_info& _ty) = 0;


		template<typename _MsgTy, typename... _Args>
		void add_log_message(_Args&&... _args)
		{
			static_assert(std::is_base_of<details::message_base, _MsgTy>::value, "_MsgTy must inherit message_base!");
			void* mem = _allocate_message(sizeof(_MsgTy));

			details::message_base* msg = new (mem) _MsgTy(std::forward<_Args>(_args)...);
			_post_message(msg);
		}

		virtual void init() = 0;
		virtual void exit(bool force = false) = 0;

		static core& get_core();
	protected:
		virtual void* _allocate_message(std::size_t _size) = 0;
		virtual void _post_message(details::message_base* message) = 0;
		virtual std::shared_ptr<tag> _add_default_tag(const std::type_info& _ty, const std::shared_ptr<tag>& _tag) = 0;
	};

	struct backend
	{
		backend()
		{
			core::get_core().init();
		}

		~backend()
		{
			core::get_core().exit();
		}
	};
}






#endif
