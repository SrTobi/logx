#pragma once
#ifndef _LOGX_TAG_HPP
#define _LOGX_TAG_HPP



#include <string>

namespace logx {



	class tag_base
	{
	public:
		virtual ~tag_base() {}
		virtual std::wstring name() const = 0;
	};


	template<typename _Name, typename... Args>
	class tag;



}







#endif