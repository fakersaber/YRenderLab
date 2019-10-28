#include <Public/Basic/File/File.h>




namespace YRender {
	std::string File::ReadFile(const std::string& filepath){
		std::ifstream file(filepath);
		std::stringstream filestream;
		filestream << file.rdbuf();
		file.close();
		return filestream.str();
	}
}


