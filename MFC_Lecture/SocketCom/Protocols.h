#pragma once

class ControlProtocol
{
public:
	virtual void Serialize(OUT unsigned char** msg, OUT DWORD& size) = 0;
};

class LightMessage : public ControlProtocol
{
public:
	enum Type
	{
		L_ALL_CH_12V,
		L_4CH_12V,
		L_4CH_38mA,
	};

public:
	LightMessage();
	virtual ~LightMessage();

	virtual void Serialize(OUT unsigned char** msg, OUT DWORD& size) override;

	void SetType(LightMessage::Type type);
	void SetBrightness(INT brightness);

private:
	const DWORD m_size;
	Type m_type;
	INT m_brightness;

	std::string m_strMsg;
};