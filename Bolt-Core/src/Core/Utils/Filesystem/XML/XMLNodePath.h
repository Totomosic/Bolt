#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct XMLnode;

	// Class that represents the path to a child node, use optional {index} if multiple children exist with same name. Eg. Resources/Textures/Texture{0}/Width
	class BLT_API XMLNodePath
	{
	public:
		static constexpr char NODE_DELIMETER = '/';

	private:
		mutable blt::string m_Path;
		mutable std::vector<int> m_ChildrenIndices;
		mutable const XMLnode* m_CurrentNode;
		mutable bool m_IsValid;

	public:
		XMLNodePath();
		XMLNodePath(const blt::string& path);
		XMLNodePath(const char* path);

		const blt::string& AsString() const;
		const std::vector<int>& ChildIndices() const;

		void SetPath(const blt::string& path);
		void CachePath(const XMLnode* node) const;

	private:
		void ProcessPathString(const XMLnode* rootNode) const;

	};

}