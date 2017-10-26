#include "hiredis.h"
#include <iostream>

using namespace std;

int main()
{
	redisContext*  c = redisConnect("127.0.0.1", 6379);
	if (c)
		cout << "yes\n";
	else
		cout << "\n";
	return 0;
}
