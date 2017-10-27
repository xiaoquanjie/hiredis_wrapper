#include "hiredis.h"
#include <iostream>
#include "wrapper/redis_wrapper.hpp"
#include "hiredis.h"
using namespace std;

int main()
{
	try
	{
		redisContext* context = redisConnect("fd", 12);
		//RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		cout << "yes\n";
	}
	catch (RedisException& e)
	{
		cout << e.What() << endl;
	}

	return 0;
}
