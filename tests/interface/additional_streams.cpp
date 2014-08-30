#include "test_settings.hpp"
#include "logx/config.hpp"
#include "logx/additional_streams.hpp"



template<typename T>
void check_without_as(T _val)
{
	using namespace logx::details::streaming_fallback;
	std::basic_ostringstream<logx::char_type> _stream;
	_stream << _val;

#ifdef LOGXCFG_USE_WCHAR
	BOOST_CHECK(_stream.str() == L"[unknown]");
#else
	BOOST_CHECK_EQUAL(_stream.str(), "[unknown]");
#endif
}

template<typename T>
void check_with_as(T _val, const logx::string& _ref)
{
	using namespace logx::additional_streams;
	std::basic_ostringstream<logx::char_type> _stream;
	_stream << _val;

#ifdef LOGXCFG_USE_WCHAR
	BOOST_CHECK(_stream.str() == _ref);
#else
	BOOST_CHECK_EQUAL(_stream.str(), _ref);
#endif
	check_without_as(_val);
}


LOGX_AUTO_TEST_CASE(test_additional_streaming_with_chrono)
{
	check_with_as(std::chrono::minutes(1), LOGXTXT("1min"));
	check_with_as(std::chrono::minutes(11), LOGXTXT("11min"));
	check_with_as(std::chrono::seconds(3), LOGXTXT("3sec"));
	check_with_as(std::chrono::nanoseconds(7), LOGXTXT("7ns"));
	check_with_as(std::chrono::milliseconds(-1), LOGXTXT("-1ms"));
	check_with_as(std::chrono::hours(1), LOGXTXT("1h"));

	typedef std::chrono::duration<int, std::ratio<1, 2>> half_second;
	check_with_as(half_second(4), LOGXTXT("2000ms"));
}