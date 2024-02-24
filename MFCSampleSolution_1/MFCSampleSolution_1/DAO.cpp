#include "pch.h"
#include "DAO.h"


DAO::DAO()
	: m_urbtActiveIdx(0U), m_ubitOptIdx(0U), m_uCnt(0U), m_uElapsed(0U), m_uhsbPos(0U), m_uvsbPos(0U)
{
}

DAO::~DAO()
{
}

IMPLEMENT_SERIAL(DAO, CObject, 1);

void DAO::Serialize(CArchive & ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar	<< m_urbtActiveIdx 
			<< m_ubitOptIdx
			<< m_uCnt
			<< m_uElapsed
			<< m_uhsbPos
			<< m_uvsbPos;
	}
	else
	{
		ar	>> m_urbtActiveIdx
			>> m_ubitOptIdx
			>> m_uCnt
			>> m_uElapsed
			>> m_uhsbPos
			>> m_uvsbPos;
	}
}
