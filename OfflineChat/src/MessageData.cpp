#include "MessageData.h"

namespace Chat
{

	MessageData::MessageData()
	{

	}

	MessageData::MessageData(const blt::string& timestamp, const blt::string& message)
		: m_Timestamp(timestamp), m_Message(message)
	{

	}

	const blt::string& MessageData::Timestamp() const
	{
		return m_Timestamp;
	}

	const blt::string& MessageData::Message() const
	{
		return m_Message;
	}

	void MessageData::SetTimestamp(const blt::string& timestamp)
	{
		m_Timestamp = timestamp;
	}

	void MessageData::SetMessage(const blt::string& message)
	{
		m_Message = message;
	}

	void MessageData::WriteDataFile(XMLfile& file) const
	{
		file.BeginXML("Timestamp");
		file.WriteXMLData(m_Timestamp);
		file.EndXML();
		file.BeginXML("Data");
		file.WriteXMLData(m_Message);
		file.EndXML();
	}

	MessageData MessageData::FromDataFile(XMLnode& node)
	{
		MessageData message;
		message.SetTimestamp(node.GetChild("Timestamp").Data);
		message.SetMessage(node.GetChild("Data").Data);
		return message;
	}

}