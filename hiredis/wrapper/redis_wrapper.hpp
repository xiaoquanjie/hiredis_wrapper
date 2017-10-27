#ifndef M_REDIS_WRAPPER_INCLUDE
#define M_REDIS_WRAPPER_INCLUDE

#include "../hiredis.h"
#include <string>
#include <tr1/memory>

#define shard_ptr_t std::tr1::shared_ptr

// redis异常
struct RedisException
{
	RedisException(){}

	RedisException(const char* what){
		_what.reset(new std::string(what));
	}

	std::string What(){
		if (!_what)
			return std::string();
		return *_what;
	}

private:
	shard_ptr_t<std::string> _what;
};

// redis连接
class RedisConnection
{
public:
	inline RedisConnection();

	inline RedisConnection(redisContext* context);

	bool IsConnected()const{
		return (_context != 0);
	}

private:
	redisContext* _context;
};

inline RedisConnection::RedisConnection():_context(0){}

inline RedisConnection::RedisConnection(redisContext* context)
	:_context(context){}



////////////////////////////////////////////////华丽分割线///////////////////////

// redis连接池
class RedisPool
{
public:
	static inline RedisConnection GetConnection(const std::string& ip,unsigned short port);

private:
	static inline void _freeRedisContext(redisContext* context);
};

inline RedisConnection RedisPool::GetConnection(const std::string& ip, unsigned short port)
{
	redisContext* context = redisConnect(ip.c_str(), port);
	if (!context) {
		_freeRedisContext(context);
		throw RedisException("redisConnect return null");
	}
	if (context->err != 0) {
		RedisException exc(context->errstr);
		_freeRedisContext(context);
		throw exc;
	}
	return RedisConnection(context);
}

void RedisPool::_freeRedisContext(redisContext* context)
{
	redisFree(context);
}

#endif