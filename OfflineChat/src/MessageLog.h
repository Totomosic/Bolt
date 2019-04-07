#pragma once
#include "ChatMessage.h"

namespace Chat
{

	class MessageLog
	{
	public:
		std::vector<ChatMessage> Messages;

	public:
		void WriteDataFile(XMLfile& file) const;

		static MessageLog FromDataFile(XMLnode& node);

	};

}