#pragma once
#ifndef _LOGX_ANNOTATION_HPP
#define _LOGX_ANNOTATION_HPP


#include <logx/logx_api.hpp>




namespace logx {


	class annotation
	{
	public:
		virtual ~annotation() {}
		virtual const std::string& name() const = 0;
		virtual const std::string& value() const = 0;
		virtual const std::vector<const annotation*>& annotations() const = 0;
	};



}






#endif