#pragma once
#include "ChatUser.h"
#include "MessageLog.h"

namespace Chat
{

	class ChatGroup
	{
	private:
		blt::string m_Name;
		std::vector<ChatUser> m_OtherUsers;
		MessageLog m_Messages;

	public:
		ChatGroup();
		ChatGroup(const blt::string& name);

		const blt::string& Name() const;
		const MessageLog& Messages() const;
		MessageLog& Messages();
		const std::vector<ChatUser>& Users() const;

		void SetName(const blt::string& name);
		void AddUser(const ChatUser& user);
		void WriteDataFile(XMLfile& file) const;

		static ChatGroup FromDataFile(XMLnode& node);

	};

}