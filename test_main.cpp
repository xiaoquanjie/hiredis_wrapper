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
template<typename T>
void print_container2(const T& t) {
	for (typename T::const_iterator iter = t.begin(); iter != t.end(); ++iter)
	{
		cout << iter->first << " " << iter->second << endl;
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

void hash_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hset²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.hset("myhash", "name", "1234") << endl;;
			cout << conn.hset("myhash", "score", 1) << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hget²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			int score = 0;
			std::string name;
			conn.hget("myhash", "name", name);
			conn.hget("myhash", "score", score);
			cout << name << " " << score << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hgetall²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::map<std::string, std::string> mymap;
			std::map<std::string, int> mymap2;
			conn.hgetall("myhash", mymap);
			conn.hgetall("myhash", mymap2);
			print_container2(mymap);
			print_container2(mymap2);
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hsetnx²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.hsetnx("myhash", "name", "12345") << endl;
			cout << conn.hsetnx("myhash", "weight", "123.45") << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hmset²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::map<int, int> map1;
			map1[1] = 1;
			map1[2] = 2;
			std::map<std::string, std::string> map2;
			map2["jie"] = 3;
			map2["quan"] = 4;
			conn.hmset("myhash", map1);
			conn.hmset("myhash", map2);
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hmget²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<TriangleValule<std::string, std::string> > values;
			TriangleValule<std::string, std::string> tv;
			tv._key = "name";
			values.push_back(tv);
			tv._key = "enen";
			values.push_back(tv);
			conn.hmget("myhash", values);
			for (std::vector<TriangleValule<std::string, std::string> >::iterator iter=values.begin();
				iter!=values.end();++iter)
			{
				cout << iter->Key() << " " << iter->_valid << " " << iter->_value << endl;
			}
		}*/
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hincrby²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			int value = 0;
			conn.hincrby("myhash", "score", value);
			cout << value << endl;
			
		}
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hexists²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.hexists("myhash", "score") << endl;
			cout << conn.hexists("myhash", "score2") << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hlen²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.hlen("myhash") << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hdel²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.hdel("myhash", "quan") << endl;
			cout << conn.hdel("myhash", "quan2") << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hkeys²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<std::string> vec;
			conn.hkeys("myhash", vec, std::string());
			print_container(vec);
		}*/
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hvals²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<int> vec;
			conn.hvals("myhash", vec, (int*)0);
			print_container(vec);
		}
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~end²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
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
	//rpoplpush_cmd();
	//hash_cmd();
	return 0;
}
