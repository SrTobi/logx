#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <logx.hpp>
#include <logx/sink.hpp>
#include <logx/sinks/text_sink.hpp>
#include <logx/sinks/xml_sink.hpp>
#include <logx/dump.hpp>
#include <mutex>

struct uncopyable
{
	uncopyable(){}
	uncopyable(const uncopyable&&) = delete;
	uncopyable(const uncopyable&) = delete;
	uncopyable& operator=(const uncopyable&) = delete;

private:
	std::mutex xx;
};

struct Point
{
	int x, y;
};

std::ostream& operator << (std::ostream& os, const uncopyable& c)
{
	os << "[uncopyable]";
	return os;
}

std::ostream& operator << (std::ostream& os, const Point& p)
{
	os << "(" << p.x << "," << p.y << ")";
	return os;
}

int main()
{
	logx::backend logbackend;

	logx::core::get_core().remove_all_sinks();
	auto sink = std::make_shared<logx::text_sink>(std::cout);
	logx::core::get_core().add_sink(std::bind(&logx::text_sink::on_message, sink, std::placeholders::_1));

	std::set<float> xx = { 4.0f, 9.3f, 8.4f };
	std::map<std::string, int> ages;
	ages["tobi"] = 22;
	ages["tally"] = 23;
	ages["jan"] = 34;
	std::vector<int> vec = { 1, 2, 3 };

	logx::logger<logx::tags::cat> logger("test");
	auto joblogger = logger.with(logx::tags::job("job"));


	joblogger << logxINFO("test")[logx::tags::cat("xxx")];

	Point p{3, 4};
	logger << logxINFO("here comes a string: " << logx::quantum(xx.begin(), xx.end())); // << logx::sequenced(vec.begin(), vec.end()));
	logger << logxINFO("p: " << p);

	//logx::log("here comes a string: " << 1.0f << ", test, " << L"wie gehts");

	uncopyable un;
	logxWARN("here comes a string again: 1" << logx::nocp(un) << "bye");
	logxERROR("Hallo \"tobi\"" << 1 << logx::nocp(un) << " wie gehts?");

	std::cin.get();
	return 0;
}
