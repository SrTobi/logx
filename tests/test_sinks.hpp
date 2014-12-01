#pragma once
#ifndef _TEST_SINKS_HPP
#define _TEST_SINKS_HPP

#include <memory>
#include <logx.hpp>
#include <logx/sink.hpp>
#include <list>
#include "logx/config.hpp"


namespace test_sinks {

	class msg_to_string_list_sink
	{
		typedef std::function<logx::string(const logx::sink_message&)> str_func;
	public:
		msg_to_string_list_sink(str_func _sink = &msg_to_string_list_sink::_default_func)
			: mSinkFunc(_sink)
		{
		}

		void on_msg(const logx::sink_message& msg)
		{
			mMsgList.push_back(mSinkFunc(msg));
		}

		logx::string pop_front()
		{
			BOOST_REQUIRE(!mMsgList.empty());
			auto r = mMsgList.front();
			mMsgList.pop_front();
			return r;
		}

	private:
		static logx::string _default_func(const logx::sink_message& msg)
		{
			return msg.msg();
		}

	private:
		str_func mSinkFunc;
		std::list<logx::string> mMsgList;
	};



}

#endif