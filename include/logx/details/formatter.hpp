#pragma once
#ifndef _LOGX_FORMATTER_HPP
#define _LOGX_FORMATTER_HPP

#include <string>
#include <vector>

namespace logx {
	namespace details {

		static std::wstring format(const std::wstring& _str, const std::vector<const std::wstring>& _args)
		{
			std::wstring _result;

			for (auto it = _str.begin(); it != _str.end(); ++it)
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
					}
				}
				else{
					_result.push_back(*it);
				}
			}



			return _result;
		}







	}
}




#endif