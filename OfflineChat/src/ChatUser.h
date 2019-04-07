#pragma once
#include "Bolt.h"

namespace Chat
{

	struct ChatUser
	{
	public:
		blt::string Username;
		SocketAddress Address;

	public:
		void WriteDataFile(XMLfile& file) const;

		static ChatUser FromDataFile(XMLnode& node);

	};

}