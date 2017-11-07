#include <iostream>
#include <fstream>
#include "wrapper/convert.hpp"
#include <stdio.h>

using namespace std;

void pause()
{
	int i;
	cin >> i;
}

int main2()
{
	FILE* f = fopen("log.txt", "a+");
	if (!f) {
		cout << "打开文件失败" << endl;
		return 0;
	}
	pause();
	size_t s = fwrite("123", 3, 1, f);
	cout << s << endl;
	fclose(f);
	return 0;
}