#include "pch.h"
#include "Protocols.h"

LightMessage::LightMessage()
	:m_size(23),
	m_type(LightMessage::L_4CH_12V), 
	m_brightness(0)
{
}

LightMessage::~LightMessage()
{
}

void LightMessage::Serialize(OUT unsigned char** msg, OUT DWORD& size)
{
	m_strMsg.resize(m_size, '0');
	m_strMsg[0] = '!';
	m_strMsg[m_size - 1] = '@';
	
	switch (m_type)
	{
	case L_ALL_CH_12V:
		m_strMsg.replace(1, 4, "U000");
		break;
	case L_4CH_12V:
		m_strMsg.replace(1, 4, "U412");
		break;
	case L_4CH_38mA:
		m_strMsg.replace(1, 4, "5400");
		break;
	}

	const auto&& bstr = std::to_string(m_brightness);
	if (bstr.size() == 2)
		m_strMsg.replace(5, 2, bstr);
	else 
		m_strMsg.replace(6, 1, bstr);

	*msg = (unsigned char *)m_strMsg.c_str();
	size = m_size;
}

void LightMessage::SetType(LightMessage::Type type)
{
	m_type = type;
}

void LightMessage::SetBrightness(INT brightness)
{
	m_brightness = max(min(99,brightness), 0);
}
