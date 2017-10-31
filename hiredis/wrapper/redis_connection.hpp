#ifndef M_REDIS_CONNECTION_INCLUDE
#define M_REDIS_CONNECTION_INCLUDE

#include <sstream>

//http://www.cnblogs.com/hjwublog/p/5639990.html
//https://redis.io/commands/setbit

// redis����
class RedisConnection
{
public:
	inline RedisConnection();

	inline RedisConnection(redisContext* context);

	bool connected()const {
		return (_context != 0);
	}

	// ��ʱ����
	inline bool expire(const char* key, time_t expire);
	// 1Ϊkey���ڣ�0��ʾkey������
	inline int del(const char* key);
	inline int del(const std::vector<std::string>& keys);
	inline int del(const std::list<std::string>& keys);

	template<typename T>
	inline void set(const char* key, T value);
	inline void set(const char* key, const std::string& value);
	inline void set(const char* key, const char* value, unsigned int len);
	template<int N>
	inline void set(const char* key, const char(&value)[N]);

	template<typename T>
	inline void get(const char* key, T& value);
	inline void get(const char* key, std::string& value);
	inline void get(const char* key, char* value,unsigned int len);

	inline void incrby(const char* key, int step);
	template<typename T>
	inline void incrby(const char* key, int step, T& new_value);
	inline void incr(const char* key);
	template<typename T>
	inline void incr(const char* key,T& new_value);

	template<typename T>
	inline void decrby(const char* key, int step, T& new_value);
	inline void decrby(const char* key, int step);
	template<typename T>
	inline void decr(const char* key, T& new_value);
	inline void decr(const char* key);

	// ���س���
	inline int strlen(const char* key);
	// �����³���
	inline int append(const char* key, const std::string& app_value);
	inline int append(const char* key, const char* value, unsigned int len);

	// ����key��Ӧ��ֵΪString���͵�value�����key�Ѿ������򷵻�false
	template<typename T>
	inline bool setnx(const char* key, T value);
	inline bool setnx(const char* key, const std::string& value);
	inline bool setnx(const char* key, const char* value, unsigned int len);
	template<int N>
	inline bool setnx(const char* key, const char(&value)[N]);

	template<typename T>
	inline void setex(const char* key, T value, time_t expire);
	inline void setex(const char* key, const std::string& value, time_t expire);
	inline void setex(const char* key, const char* value, unsigned int len, time_t expire);
	template<int N>
	inline void setex(const char* key, const char(&value)[N], time_t expire);

	// �����޸ĺ��ַ�������
	inline int setrange(const char* key, int beg_idx, const char* value, unsigned int len);
	inline int setrange(const char* key, int beg_idx, const std::string& value);
	template<int N>
	inline int setrange(const char* key, int beg_idx, const char(&value)[N]);

	inline void getrange(const char* key, int beg_idx, int end_idx, std::string& value);

	// ����ԭλ��ֵ��ֻ����0��1
	inline int setbit(const char* key, unsigned int offset, int value);
	inline int getbit(const char* key, unsigned int offset);

	template<typename T>
	inline int lpush(const char* key, const T& value);
	template<int N>
	inline int lpush(const char* key, const char(&value)[N]);
	inline int lpush(const char* key, const std::string& value);
	inline int lpush(const char* key, const char* value, unsigned int len);
	inline int lpush(const char* key, const std::vector<std::string>& values);
	inline int lpush(const char* key, const std::list<std::string>& values);
	template<typename T>
	inline int lpush(const char* key, const std::vector<T>& values);
	template<typename T>
	inline int lpush(const char* key, const std::list<T>& values);

	template<typename T>
	inline void lrange(const char* key, int beg_idx, int end_idx, std::list<T>& values);
	template<typename T>
	inline void lrange(const char* key, int beg_idx, int end_idx, std::vector<T>& values);
	inline void lrange(const char* key, int beg_idx, int end_idx, std::list<std::string>& values);
	inline void lrange(const char* key, int beg_idx, int end_idx, std::vector<std::string>& values);

	// ��Ԫ�ز�����ʱ����False
	template<typename T>
	inline bool lpop(const char* key, T& value);
	inline bool lpop(const char*key, std::string& value);
	inline bool lpop(const char*key, char* value, unsigned int len);

	template<typename T>
	inline int rpush(const char* key, const T& value);
	template<int N>
	inline int rpush(const char* key, const char(&value)[N]);
	inline int rpush(const char* key, const std::string& value);
	inline int rpush(const char* key, const char* value, unsigned int len);
	inline int rpush(const char* key, const std::vector<std::string>& values);
	inline int rpush(const char* key, const std::list<std::string>& values);
	template<typename T>
	inline int rpush(const char* key, const std::vector<T>& values);
	template<typename T>
	inline int rpush(const char* key, const std::list<T>& values);

	// ��Ԫ�ز�����ʱ����False
	template<typename T>
	inline bool rpop(const char* key, T& value);
	inline bool rpop(const char*key, std::string& value);
	inline bool rpop(const char*key, char* value, unsigned int len);

	inline int llen(const char* key);

	// ���Ԫ�ز����ڷ���false
	template<typename T>
	inline bool lindex(const char* key, int idx, T&value);
	inline bool lindex(const char* key, int idx, std::string&value);
	inline bool lindex(const char* key, int idx, char* value, int len);

	template<typename T>
	inline void lset(const char* key, int idx, T value);
	inline void lset(const char* key, int idx, const std::string& value);
	inline void lset(const char* key, int idx, const char* value, unsigned int len);
	template<int N>
	inline void lset(const char* key, int idx, const char(&value)[N]);

	template<typename T>
	inline int lrem(const char* key, int idx, T value);
	inline void ltrim(const char*key, int beg_idx, int end_idx);

	template<typename T1,typename T2>
	inline int linsert(const char* key, bool b_or_a, const T1& value1, const T2& value2);
	inline int linsert(const char* key, bool b_or_a, const std::string& value1, const std::string&value2);

	template<typename T>
	inline bool rpoplpush(const char*key1, const char*key2, T& value);
	inline bool rpoplpush(const char*key1, const char*key2, std::string& value);

	// ����1��ʾ���裬����0��ʾ����
	template<typename T>
	inline int hset(const char* key, const char* field, const T& value);
	inline int hset(const char* key, const char* field, const std::string& value);
	inline int hset(const char* key, const char* field, const char* value, unsigned int len);
	template<int N>
	inline int hset(const char* key, const char* field, const char(&value)[N]);

	template<typename T>
	inline void hget(const char* key, const char* field, T& value);
	inline void hget(const char* key, const char* field, std::string& value);
	inline void hget(const char* key, const char* field, char* value, unsigned int len);

	template<typename T1,typename T2>
	inline void hgetall(const char* key, std::map<T1, T2>& values);
	inline void hgetall(const char* key, std::map<std::string, std::string>& values);
	template<typename T>
	inline void hgetall(const char* key, std::map<T, std::string>& values);
	template<typename T>
	inline void hgetall(const char* key, std::map<std::string, T>& values);

	// ���key�Ѵ��ڣ�������ʧ��
	template<typename T>
	inline bool hsetnx(const char* key, const char* field, const T& value);
	inline bool hsetnx(const char* key, const char* field, const std::string& value);
	inline bool hsetnx(const char* key, const char* field, const char* value, unsigned int len);
	template<int N>
	inline bool hsetnx(const char* key, const char* field, const char(&value)[N]);

	template<typename T1, typename T2>
	inline void hmset(const char* key, const std::map<T1, T2>& values);
	inline void hmset(const char* key, const std::map<std::string, std::string>& values);
	template<typename T>
	inline void hmset(const char* key, const std::map<T, std::string>& values);
	template<typename T>
	inline void hmset(const char* key, const std::map<std::string, T>& values);

	// values��in-outֵ
	template<typename T1, typename T2>
	inline void hmget(const char* key, std::vector<TriangleValule<T1, T2> >&values);

	template<typename T>
	inline void hincrby(const char* key, const char* field, int step, T& new_value);
	inline void hincrby(const char* key, const char* field, int step);

	bool hexists(const char* key, const char*field);
	int  hlen(const char* key);
	bool hdel(const char* key, const char*field);

	// TҪ����һ������
	template<typename T>
	void hkeys(const char*key, T& values, typename T::value_type);
	template<typename T>
	void hkeys(const char*key, T& values, std::string);

	// TҪ����һ������
	template<typename T>
	void hvals(const char*key, T& values, typename T::value_type);
	template<typename T>
	void hvals(const char*key, T& values, std::string);

private:
	redisContext* _context;
};


#define M_CHECK_REDIS_CONTEXT(context)\
	if (!_context) throw RedisException(M_ERR_REDIS_NOT_CONNECTED);


#endif