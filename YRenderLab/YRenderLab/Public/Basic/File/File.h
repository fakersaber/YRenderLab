#ifndef _YRENDER_BASIC_FILE_H_
#define _YRENDER_BASIC_FILE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class File {
public:
	inline static std::string ReadFile(const std::string& filepath) {
		std::ifstream file(filepath);
		std::stringstream filestream;
		filestream << file.rdbuf();
		file.close();
		return filestream.str();
	}

	inline static std::vector<std::string> ReadAllLines(const std::string& filepath) {
		std::vector<std::string> AllLines;
		std::ifstream file(filepath);
		std::string CurLine;
		while (std::getline(file, CurLine)) {
			AllLines.emplace_back(CurLine + '\n');
		}
		file.close();
		return AllLines;
	}

private:
	File() = delete;
	~File() = delete;
};



#endif