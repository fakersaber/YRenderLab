#pragma once

#include <string>
#include <vector>

namespace YFG {
	class PassNode {
	public:
		PassNode(std::string InName, std::vector<size_t> Inputs, std::vector<size_t> Outputs) 
			: name{ std::move(InName) }
			, inputs{ std::move(Inputs) }
			, outputs{ std::move(Outputs) } 
		{

		}

	private:
		std::string name;
		std::vector<size_t> inputs;
		std::vector<size_t> outputs;
	};
}