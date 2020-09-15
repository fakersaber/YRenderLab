#pragma once

#include <string>

namespace YFG {
	class ResourceNode {
	public:
		ResourceNode(std::string&& InName)
			: name{ std::move(InName) }
		{

		}

	private:
		std::string name;
	};
}