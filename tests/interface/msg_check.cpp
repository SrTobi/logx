#include "test_settings.hpp"
#include "test_sinks.hpp"
#include <logx.hpp>
#include <logx/core.hpp>

struct SinkInitFixture
{
	std::shared_ptr<test_sinks::msg_to_string_list_sink> mSink;
	SinkInitFixture()
	{
		auto& core = logx::core::get_core();
		
		mSink = std::make_shared<test_sinks::msg_to_string_list_sink>();
		
		core.init();

		auto func = std::bind(&test_sinks::msg_to_string_list_sink::on_msg, mSink, std::placeholders::_1);
		core.add_sink(func);
	}

#ifndef LOGXCFG_USE_WCHAR
	void check(const std::string& _line)
	{
		BOOST_REQUIRE_EQUAL(_line, mSink->pop_front());
	}
#else

	void check(const std::wstring& _line)
	{
		BOOST_REQUIRE(_line == mSink->pop_front());
	}
#endif


	void check_simple_logging()
	{
		logx::log("test-1");
		check(LOGXTXT("test-1"));

		logx::log("linebreak in 3 2 1\nok");
		check(LOGXTXT("linebreak in 3 2 1\nok"));
	}

	void check_direct_arg_loggin()
	{
		logx::log("test $1", "test");
		check(LOGXTXT("test test"));

		logx::log("$1$2$3", 1, 2, 3);
		check(LOGXTXT("123"));

		logx::log("$1 $2 $3", 1, 2, 3);
		check(LOGXTXT("1 2 3"));

		logx::log("$1 $2 $3", 1, "test", 3.5);
		check(LOGXTXT("1 test 3.5"));
	}
};


LOGX_START_FIXTURE_TEST(SinkInitFixture)
	LOGX_FIXTURE_TEST(check_simple_logging);
	LOGX_FIXTURE_TEST(check_direct_arg_loggin);
LOGX_END_FIXTURE_TEST();