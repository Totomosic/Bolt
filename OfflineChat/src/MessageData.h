#pragma once
#include "Bolt.h"

namespace Chat
{

	class MessageData
	{
	private:
		blt::string m_Timestamp;
		blt::string m_Message;

	public:
		MessageData();
		MessageData(const blt::string& timestamp, const blt::string& message);

		const blt::string& Timestamp() const;
		const blt::string& Message() const;

		void SetTimestamp(const blt::string& timestamp);
		void SetMessage(const blt::string& message);
		void WriteDataFile(XMLfile& file) const;

		static MessageData FromDataFile(XMLnode& node);

	};

}