#pragma once

class CustomListItem
{
public:
	CustomListItem();
	~CustomListItem();

	void SetText(const CString& text);
	void SetChecked(bool checked);
	void SetComboBoxItems(const CStringList& items);

	void CreateControls(CWnd* pParent, const CRect& rect, int columnIndex);

private:
	CString m_text;
	bool m_checked;
	CStringList m_comboItems;
	CWnd* m_pControl;
};
