#include <iostream>
#define _AFXDLL
#include <afx.h>
#include <afxtempl.h>
using namespace std;


class Parent
{
public:
	void Print() { cout << "Parent Print" << endl; }
};

class Child : public Parent
{
};

int main()
{
	auto p = Parent();
	auto c = Child();

	p.Print();
	c.Print();

	return 0;
}
