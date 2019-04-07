#pragma once
#include "ChatGroup.h"

namespace Chat
{

	class ChatManager
	{
	private:
		ChatUser m_MyUser;
		std::vector<ChatGroup> m_Groups;
		int m_ActiveGroupIndex;

	public:
		ChatManager();
		ChatManager(const ChatUser& myUser);

		const ChatUser& MyUser() const;
		const std::vector<ChatGroup>& Groups() const;
		int ActiveGroupIndex() const;
		const ChatGroup& ActiveGroup() const;
		ChatGroup& ActiveGroup();

		void SetMyUser(const ChatUser& myUser);
		void SetActiveGroupIndex(int index);
		void WriteDataFile(XMLfile& file) const;

		static ChatManager FromDataFile(XMLnode& xml);

	};

}