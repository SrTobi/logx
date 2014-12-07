#pragma once
#ifndef _LOGX_XML_SINK_HPP
#define _LOGX_XML_SINK_HPP

#include <ostream>
#include "../tag.hpp"
#include "../sink.hpp"

namespace logx {

	template<typename Ch = char>
	class xml_sink
	{
	public:
		typedef std::basic_ostream<Ch> stream_type;
	public:
		xml_sink(stream_type& _stream, bool append = true)
			: mStream(_stream)
		{
			if (append)
				print_xml_start(_stream);
		}

		virtual ~xml_sink()
		{
			print_xml_end(mStream);
		}

		virtual void on_message(const sink_message& msg)
		{
			mStream << "\t<entry>\n\t\t<msg>";
			xml_write_escaped(mStream, msg.msg());
			mStream << "</msg>" << std::endl;

			for (const tag* t : msg.tags())
			{
				mStream << "\t\t<tag name=\"";
				xml_write_escaped(mStream, t->name());
				mStream << "\">";
				xml_write_escaped(mStream, t->value());
				mStream << "</tag>\n";
			}

			mStream << "\t</entry>\n";
		}


	private:

		static void xml_write_escaped(stream_type& _stream, const string& _text)
		{
			for (auto c : _text)
			{
				switch (c)
				{
				case '<':	_stream << "&lt;";		break;
				case '>':	_stream << "&gt;";		break;
				case '\"':	_stream << "&quot;";	break;
				case '\'':	_stream << "&apos";		break;
				case '&':	_stream << "&amp;";		break;
				default:
					_stream << c;
				}
			}
		}


		static void print_xml_start(stream_type& _stream)
		{
			_stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
			_stream << "<log>\n";
		}

		static void print_xml_end(stream_type& _stream)
		{
			_stream << "</log>";
		}
	private:
		stream_type& mStream;
	};





}



#endif