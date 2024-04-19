#include "pch.h"
#include "CustomListItem.h"
#include "CustomListCtrl.h"


CustomListCtrl::CustomListCtrl()
{
}

CustomListCtrl::~CustomListCtrl()
{
	for (int i = 0; i < m_items.GetCount(); ++i)
	{
		delete m_items[i];
	}
}

void CustomListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// 초기화 작업
}

void CustomListCtrl::SetColumnType(int columnIndex, ColumnType type)
{
	if (columnIndex >= 0 && columnIndex < m_columnTypes.GetCount())
	{
		m_columnTypes[columnIndex] = type;
	}
}

void CustomListCtrl::SetColumnComboBoxItems(int columnIndex, const CStringList& items)
{
	if (columnIndex >= 0 && columnIndex < m_items.GetCount())
	{
		m_items[columnIndex]->SetComboBoxItems(items);
	}
}

void CustomListCtrl::Add(CustomListItem * item)
{
	m_items.Add(item);
}

void CustomListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CListCtrl::DrawItem(lpDrawItemStruct);

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	CRect rect(lpDrawItemStruct->rcItem);
	int columnIndex = 0;

	for (auto pos = 0; pos < m_items.GetUpperBound();pos++)
	{
		CustomListItem* pItem = m_items.GetAt(pos);
		CRect itemRect(rect);
		itemRect.right = itemRect.left + GetColumnWidth(columnIndex);

		if (pItem != nullptr)
		{
			pItem->CreateControls(this, itemRect, m_columnTypes[columnIndex]);
		}

		rect.left = itemRect.right;
		columnIndex++;
	}

	dc.Detach();
}