#ifndef M_REDIS_CONNECTION_INCLUDE
#define M_REDIS_CONNECTION_INCLUDE

#include <sstream>

//http://www.cnblogs.com/hjwublog/p/5639990.html
//https://redis.io/commands/setbit

// redis连接
class RedisConnection
{
public:
	inline RedisConnection();

	inline RedisConnection(redisContext* context);

	bool IsConnected()const {
		return (_context != 0);
	}

	// 超时命令
	inline bool ExpireCommand(const char* key, time_t expire);

	template<typename T>
	inline void SetCommand(const char* key, T value);
	inline void SetCommand(const char* key, const std::string& value);
	inline void SetCommand(const char* key, const char* value, unsigned int len);
	template<int N>
	inline void SetCommand(const char* key, const char(&value)[N]);

	template<typename T>
	inline void GetCommand(const char* key, T& value);
	inline void GetCommand(const char* key, std::string& value);
	inline void GetCommand(const char* key, char* value,unsigned int len);

	inline void IncrbyCommand(const char* key, int step);
	template<typename T>
	inline void IncrbyCommand(const char* key, int step, T& new_value);
	inline void IncrCommand(const char* key);
	template<typename T>
	inline void IncrCommand(const char* key,T& new_value);

	template<typename T>
	inline void DecrbyCommand(const char* key, int step, T& new_value);
	inline void DecrbyCommand(const char* key, int step);
	template<typename T>
	inline void DecrCommand(const char* key, T& new_value);
	inline void DecrCommand(const char* key);

	// 返回长度
	inline int StrlenCommand(const char* key);
	// 返回新长度
	inline int AppendCommand(const char* key, const std::string& app_value);
	inline int AppendCommand(const char* key, const char* value, unsigned int len);

	// 设置key对应的值为String类型的value，如果key已经存在则返回false
	template<typename T>
	inline bool SetnxCommand(const char* key, T value);
	inline bool SetnxCommand(const char* key, const std::string& value);
	inline bool SetnxCommand(const char* key, const char* value, unsigned int len);
	template<int N>
	inline bool SetnxCommand(const char* key, const char(&value)[N]);

	template<typename T>
	inline void SetexCommand(const char* key, T value, time_t expire);
	inline void SetexCommand(const char* key, const std::string& value, time_t expire);
	inline void SetexCommand(const char* key, const char* value, unsigned int len, time_t expire);
	template<int N>
	inline void SetexCommand(const char* key, const char(&value)[N], time_t expire);

	// 返回修改后字符串长度
	inline int SetRangeCommand(const char* key, int beg_idx, const char* value, unsigned int len);
	inline int SetRangeCommand(const char* key, int beg_idx, const std::string& value);
	template<int N>
	inline int SetRangeCommand(const char* key, int beg_idx, const char(&value)[N]);

	inline void GetRangeCommand(const char* key, int beg_idx, int end_idx, std::string& value);

	// 返回原位的值，只会是0和1
	inline int SetbitCommand(const char* key, unsigned int offset, int value);
	inline int GetbitCommand(const char* key, unsigned int offset);

private:
	redisContext* _context;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline RedisConnection::RedisConnection() 
	:_context(0) {}

inline RedisConnection::RedisConnection(redisContext* context)
	: _context(context) {}

#define M_CHECK_REDIS_CONTEXT(context)\
	if (!_context) throw RedisException(M_ERR_REDIS_NOT_CONNECTED);

inline bool RedisConnection::ExpireCommand(const char* key, time_t expire)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "EXPIRE " + std::string(key) + " %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), expire);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int success_flag = 0;
	RedisException error;
	do 
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if (reply->type != REDIS_REPLY_INTEGER) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		success_flag = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return (success_flag == 1 ? true : false);
}

#endif