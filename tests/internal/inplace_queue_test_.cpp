#include "inplace_queue.hpp"

#include <testx/testx.hpp>



TESTX_AUTO_TEST_CASE(simple_inplace_queue_test)
{
	using namespace logx::impl;

	inplace_queue<int> queue;

	{
		auto p = queue.emplace<int>(5);

		*p = 2;
	}
}


