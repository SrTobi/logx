#include <boost/date_time/posix_time/posix_time.hpp>
#include "logx/stdtags.hpp"



namespace logx {

	namespace tags {

		thread::thread()
			: mThreadId(std::this_thread::get_id())
		{
		}


		thread::thread(std::thread::id _id)
			: mThreadId(_id)
		{
		}

		string thread::name() const
		{
			return "thread";
		}

		string thread::value() const
		{
			std::ostringstream stream;
			stream << mThreadId;
			return stream.str();
		}


		namespace details {
			using namespace boost::posix_time;
			const ptime GEpoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));

			int64_t GetCurrentTimeSinceEpoch()
			{
				auto time = microsec_clock::local_time();
				time_duration time_since_epoch = time - GEpoch;
				return time_since_epoch.total_milliseconds();
			}

			boost::posix_time::ptime ConvertRawTicksToPTime(int64_t _ticks)
			{
				time_duration time_since_epoch = milliseconds(_ticks);
				return GEpoch + time_since_epoch;
			}
		}

		time::time()
			: mTime(details::GetCurrentTimeSinceEpoch())
		{
		}

		logx::string time::name() const
		{
			return "time";
		}

		string time::value() const
		{
			using namespace boost::posix_time;

			auto t = details::ConvertRawTicksToPTime(mTime);

			auto dur = time_duration(t.time_of_day());
			std::ostringstream os;

			os << std::setfill('0');
			os << std::setw(2) << dur.hours() << ':';
			os << std::setw(2) << dur.minutes() << ':';
			os << std::setw(2) << dur.seconds();

			return os.str();
		}


	}
}