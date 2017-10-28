#ifndef M_REDIS_POOL_INCLUDE
#define M_REDIS_POOL_INCLUDE

// redisÁ¬½Ó³Ø
class RedisPool
{
public:
	static inline RedisConnection GetConnection(const std::string& ip, unsigned short port);

private:
	static inline void _freeRedisContext(redisContext* context);
};

inline RedisConnection RedisPool::GetConnection(const std::string& ip, unsigned short port)
{
	redisContext* context = redisConnect(ip.c_str(), port);
	if (!context) {
		_freeRedisContext(context);
		throw RedisException(M_ERR_REDIS_CONNECT_FAIL);
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