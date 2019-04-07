#pragma once
#include "ChatUser.h"
#include "MessageData.h"

namespace Chat
{

	struct ChatMessage
	{
	public:
		ChatUser From;
		MessageData Message;

	public:
		void WriteDataFile(XMLfile& file) const;

		static ChatMessage FromDataFile(XMLnode& node);

	};

}