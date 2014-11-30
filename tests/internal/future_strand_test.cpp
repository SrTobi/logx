#include <testx/testx.hpp>
#include "future_strand.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <thread>




TESTX_AUTO_TEST_CASE(simple_future_strand_test)
{
	boost::asio::io_service service;
	logx::details::future_strand strand(service);

	testx::MockObserver<int> mo;
	mo.set() << 1 << 2 << 3 << 4 << 5;


	auto a = strand.wrap([&mo](int i) { mo.expect(1); });
	std::function<void(int, int)> b = strand.wrap([&mo](int x, int y) { mo.expect(2); });
	auto c = strand.wrap([&mo](int i) { mo.expect(3); });
	auto d = strand.wrap([&mo](int i) { mo.expect(4); });
	auto e = strand.wrap([&]() { mo.expect(5); });

	b(2, 3);
	d(3);
	a(2);
	e();
	c(3);

	service.run();
}

TESTX_AUTO_TEST_CASE(async_future_strand_test)
{
	boost::asio::io_service service;
	logx::details::future_strand strand(service);

	testx::MockObserver<int> mo;

	std::unique_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(service));

	std::thread tha(boost::bind(&boost::asio::io_service::run, &service));
	std::thread thb(boost::bind(&boost::asio::io_service::run, &service));
	std::thread thc(boost::bind(&boost::asio::io_service::run, &service));

	for (int i = 0; i < 10000; ++i)
	{
		mo.set() << i;
		std::function<void()> w = strand.wrap([i, &mo]()
					{
						mo.expect(i);
					});

		service.post([w](){

			w();

		});
	}

	work.reset();

	service.run();
	tha.join();
	thb.join();
	thc.join();




}