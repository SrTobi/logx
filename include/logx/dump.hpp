#pragma once
#ifndef _LOGX_DUMP_HPP
#define _LOGX_DUMP_HPP

#include <ostream>
#include "config.hpp"

namespace logx {

	namespace details {


		template<typename T, typename OutputTag>
		class container_arg
		{
		public:
			container_arg(container_arg&& _old)
				: mData(std::move(_old.mData))
			{
			}

			template<typename Iter>
			container_arg(Iter _begin, Iter _end)
				: mData(_begin, _end)
			{
			}

			typename std::vector<T>::const_iterator begin() const
			{
				return mData.cbegin();
			}

			typename std::vector<T>::const_iterator end() const
			{
				return mData.cend();
			}
		private:
			std::vector<T> mData;
		};

		struct map_iterator_tag;
		struct sequence_iterator_tag;
		struct set_iterator_tag;
	}

	template<typename Iter>
	details::container_arg<typename std::iterator_traits<Iter>::value_type, details::map_iterator_tag> mapped(Iter _begin, Iter _end)
	{
		return details::container_arg<typename std::iterator_traits<Iter>::value_type, details::map_iterator_tag>(_begin, _end);
	}

	template<typename Iter>
	details::container_arg<typename std::iterator_traits<Iter>::value_type, details::sequence_iterator_tag> sequenced(Iter _begin, Iter _end)
	{
		return details::container_arg<typename std::iterator_traits<Iter>::value_type, details::sequence_iterator_tag>(_begin, _end);
	}

	template<typename Iter>
	details::container_arg<typename std::iterator_traits<Iter>::value_type, details::set_iterator_tag> quantum(Iter _begin, Iter _end)
	{
		return details::container_arg<typename std::iterator_traits<Iter>::value_type, details::set_iterator_tag>(_begin, _end);
	}
}

namespace std {
	template<typename Ch, typename It>
	std::basic_ostream<Ch>& operator << (std::basic_ostream<Ch>& _stream, const logx::details::container_arg<It, logx::details::sequence_iterator_tag>& _it)
	{
		auto begin = _it.begin();
		auto end = _it.end();
		_stream << Ch('[');
		if (begin != end)
		{
			_stream << *begin;

			while (++begin != end)
			{
				_stream << Ch(',') << Ch(' ');
				_stream << *begin;
			}
		}
		_stream << Ch(']');
		return _stream;
	}

	template<typename Ch, typename It>
	std::basic_ostream<Ch>& operator << (std::basic_ostream<Ch>& _stream, const logx::details::container_arg<It, logx::details::map_iterator_tag>& _it)
	{
		auto begin = _it.begin();
		auto end = _it.end();
		_stream << Ch('[');
		if (begin != end)
		{
			_stream << begin->first << Ch(' ') << Ch('=') << Ch('>') << Ch(' ') << begin->second;

			while (++begin != end)
			{
				_stream << Ch(',') << Ch(' ');
				_stream << begin->first << Ch(' ') << Ch('=') << Ch('>') << Ch(' ') << begin->second;
			}
		}
		_stream << Ch(']');
		return _stream;
	}

	template<typename Ch, typename It>
	std::basic_ostream<Ch>& operator << (std::basic_ostream<Ch>& _stream, const logx::details::container_arg<It, logx::details::set_iterator_tag>& _it)
	{
		auto begin = _it.begin();
		auto end = _it.end();
		_stream << Ch('[');
		if (begin != end)
		{
			_stream << *begin << Ch(' ') << Ch('=') << Ch('>') << Ch(' ');

			while (++begin != end)
			{
				_stream << *begin << Ch(' ') << Ch('=') << Ch('>') << Ch(' ');
				_stream << *begin;
			}
		}
		_stream << Ch(']');
		return _stream;
	}
}


#endif
