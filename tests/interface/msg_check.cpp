#include <testx/testx.hpp>
#include "../test_sinks.hpp"
#include <logx.hpp>
#include <logx/core.hpp>

struct SinkInitFixture
{
	std::shared_ptr<test_sinks::msg_to_string_list_sink> mSink;
	SinkInitFixture()
	{
		auto& core = logx::core::get_core();
		
		mSink = std::make_shared<test_sinks::msg_to_string_list_sink>();
		
		core.remove_all_sinks();

		auto func = std::bind(&test_sinks::msg_to_string_list_sink::on_msg, mSink, std::placeholders::_1);
		core.add_sink(func);
	}

	void check(const std::string& _line)
	{
		BOOST_REQUIRE_EQUAL(_line, mSink->pop_front());
	}

	void check_simple_logging()
	{
		logx::log() << "test-1" << logx::end;
		check("test-1");

		logx::log() << "linebreak in 3 2 1\nok" << logx::end;
		check("linebreak in 3 2 1\nok");
	}

};


TESTX_START_FIXTURE_TEST(SinkInitFixture)
	TESTX_FIXTURE_TEST(check_simple_logging);
TESTX_END_FIXTURE_TEST();