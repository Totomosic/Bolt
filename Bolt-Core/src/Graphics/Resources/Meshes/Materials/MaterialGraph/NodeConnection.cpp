#include "bltpch.h"
#include "NodeConnection.h"
#include "MaterialNode.h"

namespace Bolt
{

	NodeConnection::NodeConnection() : NodeConnection(nullptr, -1)
	{
	
	}

	NodeConnection::NodeConnection(const MaterialNode* node, int outputIndex)
		: m_Node(node), m_OutputIndex(outputIndex)
	{
	
	}

	const MaterialNode* NodeConnection::GetNode() const
	{
		return m_Node;
	}

	int NodeConnection::GetOutputIndex() const
	{
		return m_OutputIndex;
	}

	const OutputPort& NodeConnection::GetOutputPort() const
	{
		return GetNode()->GetOutputPort(GetOutputIndex());
	}

	bool NodeConnection::IsValid() const
	{
		return m_Node != nullptr && (GetOutputIndex() >= 0 && GetOutputIndex() < GetNode()->GetOutputCount());
	}

}