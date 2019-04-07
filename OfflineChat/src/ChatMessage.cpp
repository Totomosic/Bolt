#include "ChatMessage.h"

namespace Chat
{

	void ChatMessage::WriteDataFile(XMLfile& file) const
	{
		file.BeginXML("Message");
		From.WriteDataFile(file);
		Message.WriteDataFile(file);
		file.EndXML();
	}

	ChatMessage ChatMessage::FromDataFile(XMLnode& node)
	{
		ChatMessage message;
		message.From = ChatUser::FromDataFile(node.GetChild("User"));
		message.Message = MessageData::FromDataFile(node);
		return message;
	}

}