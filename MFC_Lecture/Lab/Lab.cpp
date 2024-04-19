#include <iostream>
#define _AFXDLL
#include <afx.h>
#include <afxtempl.h>
#include <vector>
#include <stack>
#include <math.h>
#include <type_traits>
#include <iostream>
using namespace std;


int main()
{
	std::cout << "remove_extent_t<int> == "
		<< typeid(std::remove_extent_t<int>).name()
		<< std::endl;
	std::cout << "remove_extent_t<int[5]> == "
		<< typeid(std::remove_extent_t<int[5]>).name()
		<< std::endl;
	std::cout << "remove_extent_t<int[5][10]> == "
		<< typeid(std::remove_extent_t<int[5][10]>).name()
		<< std::endl;

	std::cout << "remove_all_extents<int> == "
		<< typeid(std::remove_all_extents_t<int>).name()
		<< std::endl;
	std::cout << "remove_all_extents_t<int[5]> == "
		<< typeid(std::remove_all_extents_t<int[5]>).name()
		<< std::endl;
	std::cout << "remove_all_extents_t<int[5][10]> == "
		<< typeid(std::remove_all_extents_t<int[5][10]>).name()
		<< std::endl;
	return 0;
}
