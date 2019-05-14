#pragma once
#include "Nodes/MasterNode.h"

namespace Bolt
{

	class BLT_API MasterNodeSet
	{
	private:
		std::vector<MasterNode> m_MasterNodes;

	public:
		MasterNodeSet();

		int GetNodeCount() const;
		const MasterNode& GetNode(int index) const;
		ShaderVariablePtr GetNodeVariable(int index) const;

	};

}