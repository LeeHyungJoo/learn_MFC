#pragma once
class CustomListItem;
class CustomListCtrl : public CListCtrl
{
public:
	CustomListCtrl();
	virtual ~CustomListCtrl();

	void SetColumnType(int columnIndex, ColumnType type);
	void SetColumnComboBoxItems(int columnIndex, const CStringList& items);
	void Add(CustomListItem* item);

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
	CArray<ColumnType, ColumnType&> m_columnTypes;
	CArray<CustomListItem*, CustomListItem*> m_items;
};

