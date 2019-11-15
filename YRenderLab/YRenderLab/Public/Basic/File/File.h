#ifndef _YRENDER_BASIC_FILE_H_
#define _YRENDER_BASIC_FILE_H_

#include <iostream>
#include <fstream>
#include <sstream>
namespace YRender {
	class File {
	public:
		static std::string ReadFile(const std::string& filepath);

	private:
		File() = delete;
		~File() = delete;
	};
}


#endif