#pragma once
#ifndef _LOGX_HPP
#define _LOGX_HPP



#include "logx/config.hpp"
#include "logx/log.hpp"
#include "logx/tag.hpp"
#include "logx/stdtags.hpp"



namespace _logx_helper_ns = logx;
#define logxLOG(...) _logx_helper_ns::log(__VA_ARGS__)


#endif