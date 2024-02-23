#pragma once

class DAO : public CObject
{
public:
	DECLARE_SERIAL(DAO);
	DAO();
	virtual ~DAO();

	void Serialize(CArchive& ar);

public:
	UINT m_urbtActiveIdx;
	UINT m_ubitOptIdx;
	UINT m_uCnt;
	UINT m_uElapsed;
	UINT m_uhsbPos;
	UINT m_uvsbPos;
};