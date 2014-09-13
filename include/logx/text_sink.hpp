#pragma once
#ifndef _LOGX_TEXT_SINK_HPP
#define _LOGX_TEXT_SINK_HPP

#include <memory>
#include <iostream>
#include <fstream>
#include "config.hpp"
#include "sink.hpp"
#include "stdtags.hpp"

namespace logx {

	namespace details {

		template<typename Ch = char_type>
		class text_sink_impl
		{
		public:
			typedef std::basic_ostream<Ch> stream_type;
		public:
			text_sink_impl(stream_type& _stream)
				: mStream(_stream)
				, show_category(true)
				, show_loglevel(true)
			{
			}

			virtual ~text_sink_impl()
			{}

			virtual void on_message(const sink_message& msg)
			{
				if (show_category || show_loglevel)
				{
					const tags::cat* cat_tag = msg.get_tag<tags::cat>();
					const tags::log_level* ll_tag = msg.get_tag<tags::log_level>();

					if (cat_tag)
						mStream << char_type('[') << cat_tag->value() << char_type(']');

					if (ll_tag)
						mStream << ll_tag->value() << char_type(':') << char_type(' ');
				}

				mStream << msg.msg() << std::endl;
			}

			bool show_category;
			bool show_loglevel;
		private:
			stream_type& mStream;
		};
	}

	typedef details::text_sink_impl<> text_sink;
}





#endif