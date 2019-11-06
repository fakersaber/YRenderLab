#include <Public/Basic/StrApi.h>

namespace YRender {
	namespace StrAPI {
		static const std::vector<std::string> Split(const std::string& str, const std::string& separator) {
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

		bool Is_suffix(const std::string& str, const std::string& suffix){
			return suffix == str.substr(str.size() - suffix.size(), suffix.size()) ;
		}

		bool Is_prefix(const std::string& str, const std::string& prefix) {
			return prefix == str.substr(0,prefix.size());
		}
		//const std::vector<std::string> Split(const std::string& str, const char ch){
		//	std::vector<std::string> rst;
		//	return rst;
		//}		
	}
}