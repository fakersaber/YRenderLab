#include <Public/Basic/StrApi.h>
#include <algorithm>
#include <cassert>

namespace StrAPI {
	const std::vector<std::string> Split(const std::string& str, const std::string& separator) {
		//如果没有找到separator直接返回str
		std::vector<std::string> rst;
		if (separator.empty())
			return rst;
		size_t begin = 0;
		while (true) {
			size_t targetBeginIndex = str.find(separator, begin);
			if (targetBeginIndex == std::string::npos) {
				rst.emplace_back(str.substr(begin, str.size() - begin));
				break;
			}
			rst.emplace_back(str.substr(begin, targetBeginIndex - begin));
			begin += separator.size();
		}
		return rst;
	}

	const std::vector<std::string> Split(const std::string& str, const char ch) {
		std::vector<std::string> rst;
		size_t lastpos = str.find_first_not_of(ch, 0);
		size_t pos = str.find_first_of(ch, lastpos);
		while (lastpos != std::string::npos || pos != std::string::npos) {
			rst.emplace_back(str.substr(lastpos, pos - lastpos));
			lastpos = str.find_first_not_of(ch, pos);
			pos = str.find_first_of(ch, lastpos);
		}
		return rst;
	}

	bool Is_suffix(const std::string& str, const std::string& suffix) {
		assert(str.size() - suffix.size() >= 0);
		return suffix == str.substr(str.size() - suffix.size(), suffix.size());
	}

	bool Is_prefix(const std::string& str, const std::string& prefix) {
		return prefix == str.substr(0, std::min(str.size(), prefix.size()));
	}

	//只返回第一次两组之间的字符串
	std::string Between(const std::string& str, const char begin, const char end) {
		std::string FindResult;
		auto beginIndex = str.find_first_of(begin);
		if (beginIndex == std::string::npos)
			return FindResult;
		auto endIndex = str.find_first_of(end, beginIndex + 1);
		if (endIndex == std::string::npos)
			return FindResult;
		FindResult = str.substr(beginIndex + 1, endIndex - (beginIndex + 1));
		return FindResult;
	}

	//delete str from end
	const std::string StrAPI::DelTailAfter(const std::string & str, char c) {
		for (size_t i = str.size() - 1; i >= 0; i--) {
			if (str[i] == c)
				return str.substr(0, i);
		}
		return str;
	}
}