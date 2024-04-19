#include "pch.h"
#include "CustomListItem.h"


CustomListItem::CustomListItem() : m_pControl(nullptr), m_checked(false)
{
}

CustomListItem::~CustomListItem()
{
	if (m_pControl != nullptr)
	{
		m_pControl->DestroyWindow();
		delete m_pControl;
	}
}

void CustomListItem::SetText(const CString& text)
{
	m_text = text;
}

void CustomListItem::SetChecked(bool checked)
{
	m_checked = checked;
}

void CustomListItem::SetComboBoxItems(const CStringList& items)
{
	m_comboItems.RemoveAll();
	m_comboItems.AddTail(const_cast<CStringList*>(&items));
}

void CustomListItem::CreateControls(CWnd* pParent, const CRect& rect, int columnIndex)
{
	switch (columnIndex)
	{
	case TEXT_COLUMN:
		m_pControl = new CStatic();
		break;
	case CHECKBOX_COLUMN:
		m_pControl = new CButton();
		static_cast<CButton*>(m_pControl)->Create(_T("Check"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, rect, pParent, 0);
		break;
	case COMBOBOX_COLUMN:
		m_pControl = new CComboBox();
		static_cast<CComboBox*>(m_pControl)->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS, rect, pParent, 0);
		for (POSITION pos = m_comboItems.GetHeadPosition(); pos != nullptr;)
		{
			static_cast<CComboBox*>(m_pControl)->AddString(m_comboItems.GetNext(pos));
		}
		break;
	default:
		break;
	}

	if (m_pControl != nullptr)
	{
		m_pControl->MoveWindow(&rect);
	}
}