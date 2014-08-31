#pragma once
#ifndef _LOGX_FORMATTER_HPP
#define _LOGX_FORMATTER_HPP

#include <string>
#include <vector>

namespace logx {
	namespace details {

		static string format(const string& _str, const std::vector<string>& _args)
		{
			string _result;

			for (auto it = _str.begin(); it != _str.end();)
			{
				if (*it == LOGXTXT('$') && std::next(it) != _str.end())
				{
					int arg = -1;
					++it;
					if (*it != LOGXTXT('$'))
					{
						while (it != _str.end() && *it >= LOGXTXT('0') && *it <= LOGXTXT('9'))
						{
							arg += *it - LOGXTXT('0');
							++it;
						}

						if (arg >= 0 && arg < int(_args.size()))
						{
							_result += _args[arg];
						}

						if (it == _str.end())
							return _result;
					}
					else{
						_result.push_back(LOGXTXT('$'));
						++it;
					}
				}
				else{
					_result.push_back(*it);
					++it;
				}
			}



			return _result;
		}







	}
}




#endif
