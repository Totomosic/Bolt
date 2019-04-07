#include "ChatGroup.h"

namespace Chat
{

	ChatGroup::ChatGroup()
	{

	}

	ChatGroup::ChatGroup(const blt::string& name)
		: m_Name(name), m_OtherUsers(), m_Messages()
	{

	}

	const blt::string& ChatGroup::Name() const
	{
		return m_Name;
	}

	const MessageLog& ChatGroup::Messages() const
	{
		return m_Messages;
	}

	MessageLog& ChatGroup::Messages()
	{
		return m_Messages;
	}

	const std::vector<ChatUser>& ChatGroup::Users() const
	{
		return m_OtherUsers;
	}

	void ChatGroup::SetName(const blt::string& name)
	{
		m_Name = name;
	}

	void ChatGroup::AddUser(const ChatUser& user)
	{
		m_OtherUsers.push_back(user);
	}

	void ChatGroup::WriteDataFile(XMLfile& file) const
	{
		file.BeginXML("Group", { { "name", m_Name } });
		file.BeginXML("OtherUsers");
		for (const ChatUser& user : m_OtherUsers)
		{
			user.WriteDataFile(file);
		}
		file.EndXML();
		file.BeginXML("MessageHistory");
		m_Messages.WriteDataFile(file);
		file.EndXML();
		file.EndXML();
	}

	ChatGroup ChatGroup::FromDataFile(XMLnode& node)
	{
		ChatGroup group;
		group.SetName(node.Attributes.at("name"));
		for (auto& child : node.GetChild("OtherUsers").Children)
		{
			group.AddUser(ChatUser::FromDataFile(child));
		}
		group.m_Messages = MessageLog::FromDataFile(node.GetChild("MessageHistory"));
		return group;
	}

}