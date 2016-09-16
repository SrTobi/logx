#include <testx/testx.hpp>
#include <iomanip>
#include "../test_sinks.hpp"
#include <logx.hpp>
#include <logx/core.hpp>

struct uncopyable
{
	uncopyable(int i) {}
	uncopyable(uncopyable&&) = delete;
	uncopyable(const uncopyable&) = delete;
	uncopyable& operator=(const uncopyable&) = delete;
};

std::ostream& operator << (std::ostream& os, const uncopyable& c)
{
	os << "[uncopyable]";
	return os;
}

struct movable
{
	movable(int i){}
	movable(movable&& old)
	{
		mEmpty = old.mEmpty;
		old.mEmpty = true;
	}
	movable(const movable&) = delete;
	movable& operator=(const movable&) = delete;

	bool empty() const { return mEmpty; }

private:
	bool mEmpty = false;
};

std::ostream& operator << (std::ostream& os, const movable& c)
{
	os << "[movable(" << (c.empty()? "empty": "full")<< ")]";
	return os;
}

struct allable
{
	allable(int i){}
	allable(allable&& old)
	{
		mEmpty = old.mEmpty;
		old.mEmpty = true;
		mMoveInit = true;
		mCopyInit = old.mCopyInit;
	}
	allable(const allable& other)
	{
		mEmpty = other.mEmpty;
		mCopyInit = true;
		mMoveInit = other.mMoveInit;
	}
	allable& operator=(const allable&) = delete;

	bool empty() const { return mEmpty; }
	bool wasMoveInit() const { return mMoveInit; }
	bool wasCopyInit() const { return mCopyInit; }

private:
	bool mEmpty = false;
	bool mMoveInit = false;
	bool mCopyInit = false;
};


std::ostream& operator << (std::ostream& os, const allable& c)
{
	os << std::boolalpha
		<< "[allable("
		<< (c.empty()? "empty": "full")
		<< ", copy: " << c.wasCopyInit()
		<< ", move: " << c.wasMoveInit()
		<< ")]";
	return os;
}

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

	void check_basetype_logging()
	{
		logx::log() << "test-1" << logx::end;
		check("test-1");

		logx::log() << "linebreak in 3 2 1\nok" << logx::end;
		check("linebreak in 3 2 1\nok");

		logx::log() << 1 << "," << 2 << logx::end;
		check("1,2");

		logx::log() << "begin" << std::string(" ") << "end" << logx::end;
		check("begin end");

		logx::log() << "bool: " << true << logx::end;
		check("bool: true");

		logx::log() << "void: " << (void*)0x0 << logx::end;
		check("void: 0");
	}

	void check_dummy_logging()
	{
		logx::dummy_log() << "test" << logx::end;
		logx::dummy_log() << 1 << logx::end;
	}

	void check_empty_logging()
	{
		logx::log() << logx::end;
	}

	void check_uncopyable_logging()
	{
		uncopyable ucpa(1);
		logx::log() << ucpa << logx::end;
		check("[uncopyable]");
	}

	void check_moved_movable_logging()
	{
		movable mvbl(1);
		logx::log() << std::move(mvbl) << logx::end;
		check("[movable(full)]");
		BOOST_CHECK(mvbl.empty());
	}

	void check_unmoved_movable_logging()
	{
		movable mvbl(1);
		logx::log() << mvbl << logx::end;
		check("[movable(full)]");
		BOOST_CHECK(!mvbl.empty());
	}

	void check_copy_logging()
	{
		allable allab(1);
		logx::log() << allab << logx::end;
		check("[allable(full, copy: true, move: false)]");
		BOOST_CHECK(!allab.empty());
		BOOST_CHECK(!allab.wasCopyInit());
		BOOST_CHECK(!allab.wasMoveInit());
	}

	void check_move_logging()
	{
		allable allab(1);
		logx::log() << std::move(allab) << logx::end;
		check("[allable(full, copy: false, move: true)]");
		BOOST_CHECK(allab.empty());
		BOOST_CHECK(!allab.wasCopyInit());
		BOOST_CHECK(!allab.wasMoveInit());
	}

	void check_nocopy_logging()
	{
		allable allab(1);
		logx::log() << logx::nocp(allab) << logx::end;
		check("[allable(full, copy: false, move: false)]");
		BOOST_CHECK(!allab.empty());
		BOOST_CHECK(!allab.wasCopyInit());
		BOOST_CHECK(!allab.wasMoveInit());
	}
};


TESTX_START_FIXTURE_TEST(SinkInitFixture)
	TESTX_FIXTURE_TEST(check_basetype_logging);
	TESTX_FIXTURE_TEST(check_dummy_logging);
	TESTX_FIXTURE_TEST(check_empty_logging);
	TESTX_FIXTURE_TEST(check_uncopyable_logging);
	TESTX_FIXTURE_TEST(check_moved_movable_logging);
	TESTX_FIXTURE_TEST(check_unmoved_movable_logging);
	TESTX_FIXTURE_TEST(check_copy_logging);
	TESTX_FIXTURE_TEST(check_move_logging);
	TESTX_FIXTURE_TEST(check_nocopy_logging);
TESTX_END_FIXTURE_TEST();