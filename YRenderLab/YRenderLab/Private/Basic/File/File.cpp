#include <Public/Basic/File/File.h>



std::string File::ReadFile(const std::string& filepath) {
	std::ifstream file(filepath);
	std::stringstream filestream;
	filestream << file.rdbuf();
	file.close();
	return filestream.str();
}

std::vector<std::string> File::ReadAllLines(const std::string& filepath)
{
	std::vector<std::string> AllLines;
	std::ifstream file(filepath);
	std::string CurLine;
	while (std::getline(file, CurLine)) {
		AllLines.emplace_back(CurLine + '\n');
	}
	file.close();
	return AllLines;
}





