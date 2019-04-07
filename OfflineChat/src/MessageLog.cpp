#include "MessageLog.h"

namespace Chat
{

	void MessageLog::WriteDataFile(XMLfile& file) const
	{
		for (const ChatMessage& message : Messages)
		{
			message.WriteDataFile(file);
		}
	}

	MessageLog MessageLog::FromDataFile(XMLnode& node)
	{
		MessageLog log;
		for (auto& child : node.Children)
		{
			log.Messages.push_back(ChatMessage::FromDataFile(child));
		}
		return log;
	}

}