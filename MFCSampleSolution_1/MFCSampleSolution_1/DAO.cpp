#include "pch.h"
#include "DAO.h"


DAO::DAO()
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
		ar << m_urbtActiveIdx;
	}
	else
	{
		ar >> m_urbtActiveIdx;
	}
}
