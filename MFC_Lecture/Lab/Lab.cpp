#include <iostream>
#define _AFXDLL
#include <afx.h>
#include <afxtempl.h>
using namespace std;


//class Parent
//{
//public:
//	virtual void Print() { cout << "Parent Print" << endl; }
//};
//
//class Child : public Parent
//{
//public:
//	virtual void Print() { cout << "Child Print" << endl; }
//};

const int& Test()
{
	int ret = 1234;

	return ret;
}

int main()
{
	//CString showA;
	//showA.Format(false ? L"%+.0fx" : L"%+.3fx", 0.4);
	//CString formatA;
	//formatA.Format(_T("%s"), true ? "%+.0fx" : "%+.3fx");

	//CString format;
	//format.Format(_T("%s"), formatA, 0.0f);


	std::string m_strMsg;
	m_strMsg.resize(23, '0');
	m_strMsg[0] = '!';
	m_strMsg[23 - 1] = '@';
	m_strMsg.replace( 1, 4, "U000");
	return 0;
}
