#include <iostream>
#include <algorithm>
#include <string>
#include <logx.hpp>
#include <logx/sink.hpp>



logxTAG(
	TestTag,
	(
		std::to_wstring(std::get<0>(_args))
	),
	int
);

int main()
{
	std::vector<int> vec = { 1, 2, 3 };
	logx::log("here comes a string: $1", TestTag(4));

	logx::log("here comes a string: $1, $2, $3", 1.0f, "test", L"wie gehts");

	logx::log("here comes a string again: $1", "bye");

	std::cin.get();
	return 0;
}