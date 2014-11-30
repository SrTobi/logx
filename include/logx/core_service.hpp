
#pragma once
#pragma once
#ifndef _LOGX_CORE_SERVICE_HPP
#define _LOGX_CORE_SERVICE_HPP


#include <boost/asio/io_service.hpp>
#include <logx/logx_api.hpp>

namespace logx {

	class LOGX_EXPORT core_service 
	{
	public:
		core_service(boost::asio::io_service& _service);
		~core_service();

		bool active() const;

	private:
		boost::asio::io_service& mService;
	};


}






#endif