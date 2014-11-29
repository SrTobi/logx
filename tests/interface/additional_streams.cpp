#include <testx/testx.hpp>
#include "logx/config.hpp"
#include "logx/additional_streams.hpp"



template<typename T>
void check_without_as(T _val)
{
	using namespace logx::details::streaming_fallback;
	std::ostringstream stream;
	stream << _val;

	BOOST_CHECK_EQUAL(stream.str(), "[unknown]");
}

template<typename T>
void check_with_as(T _val, const logx::string& _ref)
{
	using namespace logx::additional_streams;
	std::ostringstream stream;
	stream << _val;

	BOOST_CHECK_EQUAL(stream.str(), _ref);
	check_without_as(_val);
}


TESTX_AUTO_TEST_CASE(test_additional_streaming_with_chrono)
{
	check_with_as(std::chrono::minutes(1), "1min");
	check_with_as(std::chrono::minutes(11), "11min");
	check_with_as(std::chrono::seconds(3), "3sec");
	check_with_as(std::chrono::nanoseconds(7), "7ns");
	check_with_as(std::chrono::milliseconds(-1), "-1ms");
	check_with_as(std::chrono::hours(1), "1h");

	typedef std::chrono::duration<int, std::ratio<1, 2>> half_second;
	check_with_as(half_second(4), "2000ms");
}