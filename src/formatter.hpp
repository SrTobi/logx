#pragma once
#ifndef _LOGX_FORMATTER_HPP
#define _LOGX_FORMATTER_HPP

#include <string>
#include <vector>
#include "logx/config.hpp"

namespace logx {
	namespace impl {

		string format(const string& _str, const std::vector<string>& _args);

	}
}




#endif
