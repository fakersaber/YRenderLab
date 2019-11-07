#include <Public/Basic/StrApi.h>

namespace YRender {
	namespace StrAPI {
		const std::vector<std::string> Split(const std::string& str, const std::string& separator) {
			std::vector<std::string> rst;
			if (separator.empty())
				return rst;
			size_t begin = 0;		
			while (true) {
				size_t targetBeginIndex = str.find(separator);
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

		bool Is_suffix(const std::string& str, const std::string& suffix){
			return suffix == str.substr(str.size() - suffix.size(), suffix.size()) ;
		}

		bool Is_prefix(const std::string& str, const std::string& prefix) {
			return prefix == str.substr(0,prefix.size());
		}


	}
}