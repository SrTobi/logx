#pragma once
#ifndef _LOGX_COUT_SINK_HPP
#define _LOGX_COUT_SINK_HPP

#include <memory>
#include <iostream>
#include <fstream>
#include "config.hpp"
#include "sink.hpp"
#include "stdtags.hpp"

namespace logx {

	namespace details {

		template<typename Ch = char_type>
		class stream_sink_impl : public wrapped_sink
		{
		public:
			typedef std::basic_ostream<Ch> stream_type;
		public:
			stream_sink_impl(stream_type& _stream)
				: mStream(_stream)
				, show_category(true)
				, show_loglevel(true)
			{
			}

			virtual ~stream_sink_impl()
			{}

			virtual sink wrap() override
			{
				return std::bind(&stream_sink_impl::_message, std::static_pointer_cast<stream_sink_impl>(this->shared_from_this()), std::placeholders::_1);
			}

			bool show_category;
			bool show_loglevel;
		private:
			void _message(const sink_message& msg)
			{
				if (show_category || show_loglevel)
				{
					const tags::cat* cat_tag = nullptr;
					const tags::log_level* ll_tag = nullptr;

					for (auto cur_tag : msg.tags())
					{
						if (show_category)
						{
							auto c_tag = dynamic_cast<const tags::cat*>(cur_tag);
							if (c_tag)
								cat_tag = c_tag;
						}

						if (show_loglevel)
						{
							auto l_tag = dynamic_cast<const tags::log_level*>(cur_tag);
							if (l_tag)
								ll_tag = l_tag;
						}
					}

					if (cat_tag)
						mStream << char_type('[') << cat_tag->value() << char_type(']');

					if (ll_tag)
						mStream << ll_tag->value() << char_type(':') << char_type(' ');
				}

				mStream << msg.msg() << std::endl;
			}

		private:
			stream_type& mStream;
		};


		template<typename Ch = char_type>
		class fstream_sink_impl : public stream_sink_impl<Ch>
		{
		public:
			fstream_sink_impl()
				: stream_sink_impl<Ch>(mFileStream)
			{
			}

			fstream_sink_impl(const string& _file)
				: stream_sink_impl<Ch>(mFileStream)
			{
				mFileStream.open(_file);
			}

			const std::basic_ofstream<Ch>& file_stream() const
			{
				return mFileStream;
			}

			std::basic_ofstream<Ch>& file_stream()
			{
				return mFileStream;
			}

		private:
			std::basic_ofstream<Ch> mFileStream;
		};



	}

	typedef details::stream_sink_impl<> stream_sink;
	typedef details::fstream_sink_impl<> fstream_sink;
}





#endif