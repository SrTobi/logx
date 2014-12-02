#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <logx.hpp>
#include <logx/sink.hpp>
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

std::ostream& operator << (std::ostream& os, const uncopyable& c)
{
	os << "[uncopyable]";
	return os;
}


int main()
{
	logx::backend logger;

	std::set<float> xx = { 4.0f, 9.3f, 8.4f };
	std::map<std::string, int> ages;
	ages["tobi"] = 22;
	ages["tally"] = 23;
	ages["jan"] = 34;
	std::vector<int> vec = { 1, 2, 3 };


	logx::log("here comes a string: ", logx::mapped(ages.begin(), ages.end()), logxSOURCE);

	//logx::log("here comes a string: " << 1.0f << ", test, " << L"wie gehts");

	uncopyable un;
	logx::log("here comes a string again: 1", logx::nocp(un), "bye");

	//logx::logs() << "Hallo" << 1 << un << " wie gehts?" << logx::end;

	std::cin.get();
	return 0;
}