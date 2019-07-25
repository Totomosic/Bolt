#pragma once
#include "MaterialGraphEnums.h"
#include "OutputPort.h"

namespace Bolt
{

	class MaterialNode;

	class BLT_API NodeConnection
	{
	private:
		const MaterialNode* m_Node;
		int m_OutputIndex;

	public:
		NodeConnection();
		NodeConnection(const MaterialNode* node, int outputIndex);

		const MaterialNode* GetNode() const;
		int GetOutputIndex() const;
		const OutputPort& GetOutputPort() const;
		bool IsValid() const;

	};

}