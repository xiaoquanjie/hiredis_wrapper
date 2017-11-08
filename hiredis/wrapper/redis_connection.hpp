#ifndef M_REDIS_CONNECTION_INCLUDE
#define M_REDIS_CONNECTION_INCLUDE

#include <sstream>

//http://www.cnblogs.com/hjwublog/p/5639990.html
//https://redis.io/commands/setbit

// redis����,֧�ֶ�������
class RedisConnection
{
public:
	friend class RedisPool;

	inline RedisConnection();

	inline RedisConnection(redisContext* context);

	bool connected()const {
		return (_context != 0);
	}

	unsigned long long ConnectionId()const;

	// ��ʱ����
	inline bool expire(const char* key, time_t expire);

	// 1Ϊkey���ڣ�0��ʾkey������
	inline int del(const char* key);
	template<typename T>
	int del(const T& keys);

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
	inline int lpush(const char* key, const std::string& value);
	template<typename T>
	int lpush(const char* key, const T& values,typename T::value_type*);
	template<typename T>
	int lpush(const char* key, const T& values, std::string*);

	template<typename T>
	void lrange(const char* key, int beg_idx, int end_idx, T& values, typename T::value_type*);
	template<typename T>
	void lrange(const char* key, int beg_idx, int end_idx, T& values, std::string*);

	// ��Ԫ�ز�����ʱ����False
	template<typename T>
	inline bool lpop(const char* key, T& value);
	inline bool lpop(const char*key, std::string& value);
	inline bool lpop(const char*key, char* value, unsigned int len);

	template<typename T>
	inline int rpush(const char* key, const T& value);
	inline int rpush(const char* key, const std::string& value);
	template<typename T>
	int rpush(const char* key, const T& values, typename T::value_type*);
	template<typename T>
	int rpush(const char* key, const T& values, std::string*);

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
	void hgetall(const char* key, std::map<T1, T2>& values);
	template<typename T1, typename T2, typename T3>
	void hgetall(const char* key, T1& values,std::pair<T2, T3>*);

	// ���key�Ѵ��ڣ�������ʧ��
	template<typename T>
	inline bool hsetnx(const char* key, const char* field, const T& value);
	inline bool hsetnx(const char* key, const char* field, const std::string& value);
	inline bool hsetnx(const char* key, const char* field, const char* value, unsigned int len);
	template<int N>
	inline bool hsetnx(const char* key, const char* field, const char(&value)[N]);

	template<typename T1, typename T2>
	void hmset(const char* key, const std::map<T1, T2>& values);
	template<typename T1, typename T2, typename T3>
	void hmset(const char* key, const T1& values, std::pair<T2, T3>*);

	// values��in-outֵ
	template<typename T1, typename T2>
	inline void hmget(const char* key, std::vector<TriangleValule<T1, T2> >&values);

	template<typename T>
	inline void hincrby(const char* key, const char* field, int step, T& new_value);
	inline void hincrby(const char* key, const char* field, int step);

	bool hexists(const char* key, const char*field);
	int  hlen(const char* key);
	bool hdel(const char* key, const char*field);
	template<typename T>
	bool hdel(const char*key, const T& field);

	// TҪ����һ������
	template<typename T>
	void hkeys(const char*key, T& values, typename T::value_type*);
	template<typename T>
	void hkeys(const char*key, T& values, std::string*);

	// TҪ����һ������
	template<typename T>
	void hvals(const char*key, T& values, typename T::value_type*);
	template<typename T>
	void hvals(const char*key, T& values, std::string*);

	// ���سɹ�����ĸ���
	template<typename T>
	int sadd(const char*key, const T& value);
	int sadd(const char*key, const std::string&value);
	template<typename T>
	int sadd(const char*key, const T& values, typename T::value_type*);
	template<typename T>
	int sadd(const char*key, const T& values, std::string*);

	// TҪ����һ������
	template<typename T>
	void smembers(const char*key, T&values, typename T::value_type*);
	template<typename T>
	void smembers(const char*key, T&values, std::string*);

	template<typename T>
	bool spop(const char*key, T&value);
	bool spop(const char*key, std::string&value);
	bool spop(const char*key, char*value, unsigned int len);

	template<typename T>
	bool srem(const char*key, const T& field);
	bool srem(const char*key, const char* field);

	int scard(const char*key);
	bool sismember(const char*key, const char* field);
	template<typename T>
	bool sismember(const char*key, const T& field);
	template<typename T>
	bool smove(const char* src_key, const char* dst_key,const T& field);
	bool smove(const char* src_key, const char* dst_key, const char* field);

	template<typename T>
	bool srandmember(const char*key, T&value);
	bool srandmember(const char*key, std::string&value);
	// TҪ����һ������
	template<typename T>
	void srandmember(const char*key, T&values, int count, typename T::value_type*);
	template<typename T>
	void srandmember(const char*key, T&values, int count, std::string*);

	template<typename T>
	void sdiff(const char* key, const std::vector<std::string>& other_keys, T& values, typename T::value_type*);
	template<typename T>
	void sdiff(const char* key, const std::vector<std::string>& other_keys, T& values, std::string*);

	template<typename T>
	void sunion(const char* key, const std::vector<std::string>& other_keys, T& values, typename T::value_type*);
	template<typename T>
	void sunion(const char* key, const std::vector<std::string>& other_keys, T& values, std::string*);

	template<typename T>
	void sinter(const char* key, const std::vector<std::string>& other_keys, T& values, typename T::value_type*);
	template<typename T>
	void sinter(const char* key, const std::vector<std::string>& other_keys, T& values, std::string*);

	int sdiffstore(const char* key, const std::vector<std::string>& other_keys);
	int sunionstore(const char* key, const std::vector<std::string>& other_keys);
	int sinterstore(const char* key, const std::vector<std::string>& other_keys);

	template<typename T1,typename T2>
	int zadd(const char*key, const T1& score, const T2& member);
	template<typename T>
	int zadd(const char*key, const T& score, const std::string& member);
	// TҪ�������ף���Ԫ����pair�ṹ
	template<typename T>
	int zadd(const char*key, const T& values);

	template<typename T>
	void zrange(const char*key, int beg_idx, int end_idx, T& values, typename T::value_type*);
	template<typename T>
	void zrange(const char*key, int beg_idx, int end_idx, T& values, std::string*);

	template<typename T1,typename T2,typename T3>
	void zrangewithscores(const char* key, int beg_idx, int end_idx, T1& values,
		std::pair<T2,T3>*);
	template<typename T1, typename T2>
	void zrangewithscores(const char* key, int beg_idx, int end_idx,
		std::map<T1, T2>& values);

	template<typename T>
	bool zrem(const char*key, const T& member);
	bool zrem(const char*key, const std::string& member);
	template<typename T>
	int  zrem(const char*key, const T& members, typename T::value_type*);
	template<typename T>
	int  zrem(const char*key, const T& members, std::string*);

	template<typename T>
	void zrevrange(const char*key, int beg_idx, int end_idx, T& values, typename T::value_type*);
	template<typename T>
	void zrevrange(const char*key, int beg_idx, int end_idx, T& values, std::string*);

	template<typename T1, typename T2, typename T3>
	void zrevrangewithscores(const char* key, int beg_idx, int end_idx, T1& values,
		std::pair<T2, T3>*);
	template<typename T1, typename T2>
	void zrevrangewithscores(const char* key, int beg_idx, int end_idx, std::map<T1, T2>& values);

	bool zrank(const char* key, const char* member, int& rank);
	bool zrevrank(const char* key, const char* member, int& rank);

	void zcard(const char* key, int& count);
	template<typename T1,typename T2>
	void zcount(const char* key, T1 min, T2 max, int& count);

	template<typename T>
	void zincrby(const char* key, const char* member, T incr, T& score);

	template<typename T1,typename T2,typename T3>
	void zrangebyscore(const char* key, T1 min, T2 max, T3& values, typename T3::value_type*);
	template<typename T1, typename T2, typename T3>
	void zrangebyscore(const char* key, T1 min, T2 max, T3& values, std::string*);

	template<typename T1, typename T2, typename T3>
	void zrevrangebyscore(const char* key, T1 min, T2 max, T3& values, typename T3::value_type*);
	template<typename T1, typename T2, typename T3>
	void zrevrangebyscore(const char* key, T1 min, T2 max, T3& values, std::string*);

	template<typename T>
	void zscore(const char* key, const char* member, T& score);

private:
	redisContext* _context;
	time_t        _reconn_time; // ��������ʱ��
};


#define M_CHECK_REDIS_CONTEXT(context)\
	if (!_context) throw RedisException(M_ERR_REDIS_NOT_CONNECTED);


#endif