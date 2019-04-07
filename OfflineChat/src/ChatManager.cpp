#include "ChatManager.h"

namespace Chat
{

	ChatManager::ChatManager()
	{

	}

	ChatManager::ChatManager(const ChatUser& myUser)
		: m_MyUser(myUser), m_Groups(), m_ActiveGroupIndex(-1)
	{

	}

	const ChatUser& ChatManager::MyUser() const
	{
		return m_MyUser;
	}

	const std::vector<ChatGroup>& ChatManager::Groups() const
	{
		return m_Groups;
	}

	int ChatManager::ActiveGroupIndex() const
	{
		return m_ActiveGroupIndex;
	}

	const ChatGroup& ChatManager::ActiveGroup() const
	{
		return m_Groups.at(m_ActiveGroupIndex);
	}

	ChatGroup& ChatManager::ActiveGroup()
	{
		return m_Groups.at(m_ActiveGroupIndex);
	}

	void ChatManager::SetMyUser(const ChatUser& myUser)
	{
		m_MyUser = myUser;
	}

	void ChatManager::SetActiveGroupIndex(int index)
	{
		m_ActiveGroupIndex = index;
	}

	void ChatManager::WriteDataFile(XMLfile& file) const
	{
		file.BeginXML("Groups");
		for (const auto& group : m_Groups)
		{
			group.WriteDataFile(file);
		}
		file.EndXML();
	}

	ChatManager ChatManager::FromDataFile(XMLnode& node)
	{
		ChatManager manager;
		for (auto& child : node.Children)
		{
			manager.m_Groups.push_back(ChatGroup::FromDataFile(child));
		}
		return manager;
	}

}