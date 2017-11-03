#include <iostream>
#include "wrapper/convert.hpp"

using namespace std;

int main2()
{
	int i = 5,j;
	std::string str1, str2;
	//convert_to_t(i, str1);
	convert_to_t((std::string)str1, j);
	convert_to_t(j, str2);


	return 0;
}