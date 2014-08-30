#pragma once
#ifndef _LOGX_FORMATTER_HPP
#define _LOGX_FORMATTER_HPP

#include <string>
#include <vector>

namespace logx {
	namespace details {

		static string format(const string& _str, const std::vector<const string>& _args)
		{
			string _result;

			for (auto it = _str.begin(); it != _str.end();)
			{
				if (*it == L'$' && std::next(it) != _str.end())
				{
					int arg = -1;
					++it;
					if (*it != L'$')
					{
						while (it != _str.end() && *it >= L'0' && *it <= L'9')
						{
							arg += *it - L'0';
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
						_result.push_back(L'$');
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