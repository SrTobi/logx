#pragma once
#ifndef _LOGX_TEXT_SINK_HPP
#define _LOGX_TEXT_SINK_HPP

#include <memory>
#include <iostream>
#include <fstream>
#include "../config.hpp"
#include "../sink.hpp"
#include "../stdtags.hpp"

namespace logx {

	namespace details {

		template<typename Ch = char>
		class text_sink_impl
		{
		public:
			typedef std::basic_ostream<Ch> stream_type;
		public:
			text_sink_impl(stream_type& _stream)
				: mStream(_stream)
				, show_category(true)
				, show_loglevel(true)
				, show_time(true)
				, show_source(true)
			{
			}

			virtual ~text_sink_impl()
			{}

			virtual void on_message(const sink_message& msg)
			{
				if (show_time)
				{
					const tags::time* time_tag = msg.get_tag<tags::time>();

					if (time_tag)
						mStream << '[' << time_tag->value() << ']';
				}

				if (show_category)
				{
					const tags::cat* cat_tag = msg.get_tag<tags::cat>();

					if (cat_tag)
						mStream << '[' << cat_tag->value() << ']';
				}

				const tags::log_level* ll_tag = nullptr;
				if (show_loglevel)
				{
					ll_tag = msg.get_tag<tags::log_level>();

					if (ll_tag)
						mStream << ll_tag->value() << ':' << ' ';
				}

				mStream << msg.msg();

				if (show_source && ll_tag && ll_tag->level() >= tags::log_levels::ERR)
				{
					const tags::source* src_tag = msg.get_tag<tags::source>();

					if (src_tag)
						mStream << " [" << src_tag->value() << ']';
				}

				mStream << std::endl;
			}

			bool show_category;
			bool show_loglevel;
			bool show_time;
			bool show_source;
		private:
			stream_type& mStream;
		};
	}

	typedef details::text_sink_impl<> text_sink;
}





#endif