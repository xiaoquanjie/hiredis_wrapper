#include <iostream>
#include "wrapper/redis_wrapper.hpp"
#include "wrapper/variant.h"

using namespace std;

template<typename T>
void print_container(const T& t) {
	for (typename T::const_iterator iter=t.begin(); iter!=t.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;
}

void set_cmd()
{
	try
	{
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		conn.set("age", "xioquanjie~~");
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void setbit_cmd()
{
	try
	{
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		conn.setbit("mybit", 0, 1);
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void getbit_cmd()
{
	try
	{
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		cout << conn.getbit("mybit", 0) << endl;
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void del_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		std::vector<std::string> vec;
		vec.push_back("age");
		vec.push_back("xiao");
		cout << conn.del(vec) << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void lpush_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		std::vector<int> vec;
		vec.push_back(100);
		vec.push_back(102);
		vec.push_back(103);
		cout << conn.lpush("int_l", vec) << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void lrange_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		std::vector<int> vec;
		conn.lrange("int_l", 0, -1, vec);
		print_container(vec);
		std::vector<std::string> vec2;
		conn.lrange("int_l", 0, -1, vec2);
		print_container(vec2);

	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void lpop_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		std::string str;
		if (conn.lpop("int_l", str))
			cout << str << endl;
		else
			cout << "nil" << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void llen_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		cout << conn.llen("int_l") << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void rpush_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		std::vector<int> int_vec;
		int_vec.push_back(1);
		int_vec.push_back(2);
		cout << conn.rpush("int_l", int_vec) << endl;
		std::vector<string> str_vec;
		str_vec.push_back("a");
		str_vec.push_back("b");
		cout << conn.rpush("int_l", str_vec) << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void rpop_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		std::string str;
		if (conn.rpop("int_l", str))
			cout << str << endl;
		else
			cout << "nil" << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void lindex_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		char str[10] = { 0 };
		if (conn.lindex("int_l",0,str,9))
			cout << str << endl;
		else
			cout << "nil" << endl;
		if (conn.lindex("int_l", 100, str, 9))
			cout << str << endl;
		else
			cout << "nil" << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void lset_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		conn.lset("int_l", 0, "taoxinzhi");
		conn.lset("int_l", 1, "2222");
		conn.lset("int_l", 10, "1taoxinzhi");
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void lrem_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		cout << conn.lrem("int_l", 0, "1") << endl;
		cout << conn.lrem("int_l", 0, "3") << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void ltrim_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		conn.ltrim("int_l", 0, 1);
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void linsert_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		cout << conn.linsert("int_l", true, 4, std::string("a")) << endl;
		cout << conn.linsert("int_l", false, 4, std::string("a")) << endl;
		cout << conn.linsert("int_l", false, 10, std::string("a")) << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void rpoplpush_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		std::string str;
		cout << conn.rpoplpush("int_l","int_2",str) << endl;
		cout << str << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

int main()
{
	//set_cmd();
	//setbit_cmd();
	//getbit_cmd();
	//del_cmd();
	//lpush_cmd();
	//lrange_cmd();
	//lpop_cmd();
	//rpush_cmd();
	//rpop_cmd();
	//llen_cmd();
	//lindex_cmd();
	//lset_cmd();
	//lrem_cmd();
	//ltrim_cmd();
	//linsert_cmd();
	rpoplpush_cmd();
	return 0;
}
