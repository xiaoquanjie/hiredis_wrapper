#include <iostream>
#include "wrapper/redis_wrapper.hpp"
#include "wrapper/variant.h"
#include "thread.hpp"

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
		cout << conn.lpush("int_l", vec,(int*)0) << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}
void lrange_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		std::vector<int> vec;
		conn.lrange("int_l", 0, -1, vec,(int*)0);
		print_container(vec);
		std::vector<std::string> vec2;
		conn.lrange("int_l", 0, -1, vec2,(std::string*)0);
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
		cout << conn.rpush("int_l", int_vec,(int*)0) << endl;
		std::vector<string> str_vec;
		str_vec.push_back("a");
		str_vec.push_back("b");
		cout << conn.rpush("int_l", str_vec,(string*)0) << endl;
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
			std::vector<std::pair<std::string, string> > vec;
			conn.hgetall("myhash", mymap);
			conn.hgetall("myhash", vec, (std::pair<std::string, string>*)0);
			print_container2(mymap);
			print_container2(vec);
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hsetnx²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.hsetnx("myhash", "name", "12345") << endl;
			cout << conn.hsetnx("myhash", "weight", "123.45") << endl;
		}*/
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hmset²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::map<std::string, std::string> map2;
			map2["jie"] = 3;
			map2["quan"] = 4;
			conn.hmset("myhash", map2);

			std::vector<pair<int, int> > vec;
			vec.push_back(make_pair(1000, 11));
			vec.push_back(make_pair(1001, 12));
			conn.hmset("myhash", vec, (pair<int, int>*)0);
		}
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
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hincrby²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			int value = 0;
			conn.hincrby("myhash", "score", value);
			cout << value << endl;

		}*/
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
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hvals²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<int> vec;
			conn.hvals("myhash", vec, (int*)0);
			print_container(vec);
		}*/
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~end²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void set_command() {
	try
	{
		const char* key = "myset";
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~sadd²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.sadd(key,1) << endl;
			cout << conn.sadd(key, "2") << endl;
			cout << conn.sadd(key, std::string("3")) << endl;

			std::vector<int> int_vec;
			int_vec.push_back(4);
			int_vec.push_back(5);
			cout << conn.sadd(key, int_vec,(int*)0) << endl;

			std::vector<string> int_vec2;
			int_vec2.push_back("6");
			int_vec2.push_back("7");
			cout << conn.sadd(key, int_vec2, (string*)0) << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~smembers²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<int> int_vec;
			std::vector<string> int_vec2;
			conn.smembers(key, int_vec, (int*)0);
			conn.smembers(key, int_vec2, (string*)0);
			print_container(int_vec);
			print_container(int_vec2);
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~spop²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			int v1 = 0;
			std::string v2;
			cout << conn.spop(key, v1) << endl;
			cout << conn.spop(key, v2) << endl;
			cout << v1 << " " << v2 << endl;
			cout << conn.spop("enen", v1) << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~srem²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.srem(key, 1) << endl;
			cout << conn.srem(key, 100) << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~scard²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.scard(key) << endl;
			cout << conn.scard("enen2") << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~sismember²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.sismember(key, 2) << endl;
			cout << conn.sismember(key, "xiao") << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~somve²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.smove("myset", "myset2", 2) << endl;
			cout << conn.smove("myset", "myset2", (char*)"2") << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~sdiff²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<std::string> vec1;
			vec1.push_back("myset2");
			std::vector<int> int_vec;
			std::vector<std::string> str_vec;
			conn.sdiff(key, vec1, int_vec, (int*)0);
			conn.sdiff(key, vec1, str_vec, (string*)0);
			print_container(int_vec);
			print_container(str_vec);
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~sunion²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<std::string> vec1;
			vec1.push_back("myset2");
			std::vector<int> int_vec;
			std::vector<std::string> str_vec;
			conn.sunion(key, vec1, int_vec, (int*)0);
			conn.sunion(key, vec1, str_vec, (string*)0);
			print_container(int_vec);
			print_container(str_vec);
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~srandmember²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			int v1;
			cout << conn.srandmember(key, v1) << endl;
			string v2;
			cout << conn.srandmember(key, v2) << endl;

			std::vector<std::string> str_vec;
			conn.srandmember(key, str_vec,2,(string*)0);
			std::vector<int> int_vec;
			conn.srandmember(key, int_vec,2,(int*)0);

			cout << v1 << endl;
			cout << v2 << endl;
			print_container(str_vec);
			print_container(int_vec);
		}*/
		/*	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~sinter²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			{
				std::vector<std::string> vec1;
				vec1.push_back("myset2");
				std::vector<int> int_vec;
				std::vector<std::string> str_vec;
				conn.sinter(key, vec1, int_vec, (int*)0);
				conn.sinter(key, vec1, str_vec, (string*)0);
				print_container(int_vec);
				print_container(str_vec);
			}*/
			/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~sdiffstore²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			{
				std::vector<std::string> vec1;
				vec1.push_back("myset2");
				vec1.push_back("myset1");
				cout << conn.sdiffstore("myset3", vec1) << endl;
			}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~sunionstore²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<std::string> vec1;
			vec1.push_back("myset2");
			vec1.push_back("myset1");
			cout << conn.sunionstore("myset4", vec1) << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~sinterstore²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<std::string> vec1;
			vec1.push_back("myset2");
			vec1.push_back("myset1");
			cout << conn.sinterstore("myset5", vec1) << endl;
		}*/
	}
	catch (RedisException& e) {
		cout << e.What() << endl;
	}
}

void zset_cmd() {
	try {
		const char* key = "myzset";
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zadd²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.zadd(key, 10, "1990") << endl;
			cout << conn.zadd(key, 20, 1991) << endl;

			std::vector<std::pair<int, int> > vec;
			vec.push_back(std::make_pair(1, 1992));
			vec.push_back(std::make_pair(2, 1993));
			cout << conn.zadd(key, vec) << endl;

			std::map<string, int> m;
			m["1994"] = 1;
			m["1995"] = 2;
			cout << conn.zadd(key, m) << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zrange²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<int> vec1;
			std::vector<string> vec2;
			conn.zrange(key, 0, -1, vec1, (int*)0);
			conn.zrange(key, 0, -1, vec2, (string*)0);
			print_container(vec1);
			print_container(vec2);
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zrangewithscores²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<std::pair<int, int> > vec;
			std::map<string, int> m;
			conn.zrangewithscores(key, 0, -1, m);
			conn.zrangewithscores(key, 0, -1, vec, (std::pair<int, int>*) 0);
			print_container2(m);
			print_container2(vec);
		}*/
		/*	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zrem²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			{
				cout << conn.zrem(key, std::string("1996")) << endl;
				cout << conn.zrem(key, std::string("1995")) << endl;
				cout << conn.zrem(key, 1994) << endl;

				std::vector<int> vec1;
				vec1.push_back(1997);
				vec1.push_back(1998);
				cout << conn.zrem(key, vec1, (int*)0) << endl;

				std::vector<string> vec2;
				vec2.push_back("1999");
				vec2.push_back("2000");
				cout << conn.zrem(key, vec2, (string*)0) << endl;
			}*/
			/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zrevrange²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			{
				std::vector<int> vec1;
				std::vector<string> vec2;
				conn.zrevrange(key, 0, -1, vec1, (int*)0);
				conn.zrevrange(key, 0, -1, vec2, (string*)0);
				print_container(vec1);
				print_container(vec2);
			}*/
			/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zrevrangewithscores²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			{
				std::vector<std::pair<int, int> > vec;
				std::map<string, int> m;
				conn.zrangewithscores(key, 0, -1, m);
				conn.zrangewithscores(key, 0, -1, vec, (std::pair<int, int>*) 0);
				print_container2(m);
				print_container2(vec);
			}*/
			/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zrank²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			{
				int rank = -1, rank2 = -1;
				cout << conn.zrank(key, "1992",rank) << endl;
				cout << conn.zrank(key, "fdsf", rank2) << endl;
				cout << rank << " " << rank2 << endl;
			}*/
			/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zrevrank²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			{
				int rank = -1, rank2 = -1;
				cout << conn.zrevrank(key, "1992", rank) << endl;
				cout << conn.zrevrank(key, "fdsf", rank2) << endl;
				cout << rank << " " << rank2 << endl;
			}*/
			/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zcard²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			{
				int count;
				conn.zcard(key, count);
				cout << count << endl;
			}*/
			/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zcount²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			{
				int count;
				conn.zcount(key, 0, 100000, count);
				cout << count << endl;
			}*/
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zincrby²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			int score = 0;
			conn.zincrby(key, "1991", 1, score);
			cout << score << endl;
		}
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zscore²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			int score = 0;
			conn.zscore(key, "1991", score);
			cout << score << endl;
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zrangebyscore²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<int> vec1;
			std::vector<string> vec2;
			conn.zrangebyscore(key, 0, 100, vec1, (int*)0);
			conn.zrangebyscore(key, 0, 100, vec2, (string*)0);
			print_container(vec1);
			print_container(vec2);
		}*/
		/*cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~zrevrangebyscore²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			std::vector<int> vec1;
			std::vector<string> vec2;
			conn.zrevrangebyscore(key, 0, 100, vec1, (int*)0);
			conn.zrevrangebyscore(key, 0, 100, vec2, (string*)0);
			print_container(vec1);
			print_container(vec2);
		}*/
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}

}

void other_cmd() {
	try {
		RedisConnection conn = RedisPool::GetConnection("127.0.0.1", 6379);
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~del²âÊÔ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		{
			cout << conn.del("myhash") << endl;
			vector<string> vec;
			vec.push_back("myset2");
			vec.push_back("myset3");
			cout << conn.del(vec) << endl;
		}
	}
	catch (RedisException&e) {
		cout << e.What() << endl;
	}
}

void thread_func(void*) {

	for (int i = 0; i < 2; ++i)
	{
		RedisConnection conn1 = RedisPool::GetConnection("127.0.0.1", 6379, 0);
		RedisConnection conn2 = RedisPool::GetConnection("127.0.0.1", 6379, 1);
		cout << thread::ctid() << " " << conn1.ConnectionId() << " " << conn2.ConnectionId() << endl;
	}
	
}

int main()
{
	cout << sizeof(unsigned long long) << endl;
	thread thr1(&thread_func, 0);
	thread::sleep(200);
	thread thr2(&thread_func, 0);

	thr1.join();
	thr2.join();

	int i = 0;
	cin >> i;
	return 0;
}
