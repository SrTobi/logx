#pragma once
#ifndef _TEST_SINKS_HPP
#define _TEST_SINKS_HPP

#include <memory>
#include <logx.hpp>
#include <logx/sink.hpp>
#include <list>

namespace test_sinks {

	class msg_to_string_list_sink
	{
		typedef std::function<std::wstring(const logx::sink_message&)> str_func;
	public:
		msg_to_string_list_sink(str_func _sink = &msg_to_string_list_sink::_default_func)
			: mSinkFunc(_sink)
		{
		}

		void on_msg(const logx::sink_message& msg)
		{
			mMsgList.push_back(mSinkFunc(msg));
		}

		std::wstring pop_front()
		{
			auto r = mMsgList.front();
			mMsgList.pop_front();
			return r;
		}

	private:
		static std::wstring _default_func(const logx::sink_message& msg)
		{
			return msg.msg();
		}

	private:
		str_func mSinkFunc;
		std::list<std::wstring> mMsgList;
	};



}

#endif