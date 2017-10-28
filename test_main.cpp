#include <iostream>
#include "wrapper/redis_wrapper.hpp"
#include "wrapper/variant.h"
using namespace std;

void hiredis_wrapper_get()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		double age = 0;
		con.GetCommand("age", age);
		cout << age << endl;
	}
	catch (RedisException& e)
	{
		cout << e.What() << endl;
	}
}

void hiredis_wrapper_set()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		con.SetCommand("age", 1003.434);
	}
	catch (RedisException& e)
	{
		cout << e.What() << endl;
	}
}

void incr_cmd()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		int value = 0;
		con.IncrbyCommand("age", 1, value);
		cout << value << endl;
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void decr_cmd()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		int value = 0;
		con.DecrbyCommand("age", 2, value);
		cout << value << endl;
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void strlen_cmd()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		cout << con.StrlenCommand("age") << endl;
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void append_cmd()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		cout << con.AppendCommand("age","nihao") << endl;
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void setnx_cmd()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		int result = con.SetnxCommand("age", "3");
		cout << result << endl;
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void setex_cmd()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		con.SetCommand("age", "xiao");
		con.SetexCommand("age1", "xiao1",100);
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void getrange_cmd()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		std::string value;
		con.GetRangeCommand("age", 0, -1, value);
		cout << value << endl;
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void setrange_cmd()
{
	try
	{
		RedisConnection con = RedisPool::GetConnection("127.0.0.1", 6379);
		cout << con.SetRangeCommand("age", 0, "woaini") << endl;
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

int main()
{
	//setnx_cmd();
	//setex_cmd();
	//getrange_cmd();
	setrange_cmd();
	return 0;
}
