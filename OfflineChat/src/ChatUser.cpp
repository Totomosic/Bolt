#include "ChatUser.h"

namespace Chat
{

	void ChatUser::WriteDataFile(XMLfile& file) const
	{
		file.BeginXML("User", { { "name", Username }, { "ip", Address.ToString() } });
		file.EndXML();
	}

	ChatUser ChatUser::FromDataFile(XMLnode& node)
	{
		ChatUser user;
		user.Username = node.Attributes.at("name");
		return user;
	}

}