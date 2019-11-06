#ifndef _YRENDER_BASIC_STRAPI_H_
#define _YRENDER_BASIC_STRAPI_H_

#include <string>
#include <vector>

namespace YRender {
	namespace StrAPI {
		static const std::vector<std::string> Split(const std::string& str, const std::string& separator);
		static const std::vector<std::string> Split(const std::string& str, const char ch);

		static bool Is_suffix(const std::string& str, const std::string& suffix);
	}
}



#endif