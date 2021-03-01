#ifndef _YRENDER_BASIC_STRAPI_H_
#define _YRENDER_BASIC_STRAPI_H_

#include <string>
#include <vector>


namespace StrAPI {
	const std::vector<std::string> Split(const std::string& str, const std::string& separator);
	const std::vector<std::string> Split(const std::string& str, const char ch);
	bool Is_suffix(const std::string& str, const std::string& suffix);
	bool Is_prefix(const std::string& str, const std::string& prefix);
	std::string Between(const std::string& str, const char begin, const char end);
	const std::string DelTailAfter(const std::string & str, char c);
}




#endif