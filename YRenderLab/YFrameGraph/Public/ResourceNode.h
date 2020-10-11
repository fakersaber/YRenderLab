#pragma once

#include <string>

namespace YFG {
	class ResourceNode {
	public:
		//不管左值还是右值给定字符串总会调用一次构造函数造成内存分配或内存拷贝，所以统一用值类型
		ResourceNode(std::string InName)
			: name{ std::move(InName) } 
		{ 

		}

		const std::string& Name() const noexcept { return name; }
	private:
		std::string name;
	};
}