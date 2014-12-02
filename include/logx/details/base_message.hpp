#pragma once
#ifndef _LOGX_MESSAGE_HPP
#define _LOGX_MESSAGE_HPP


#include <string>
#include <vector>

namespace logx {
	class tag;
	class annotation;


	namespace details {

		class message_base
		{
		public:
			virtual ~message_base() {};
			virtual string msg() const = 0;
			virtual std::vector<const tag*> tags() const = 0;
			virtual std::vector<const annotation*> annotations() const = 0;
		};




	}
}



#endif
