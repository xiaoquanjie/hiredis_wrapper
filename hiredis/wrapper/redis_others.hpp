#ifndef M_REDIS_OTHERS_INCLUDE
#define M_REDIS_OTHERS_INCLUDE

inline RedisConnection::RedisConnection()
	:_context(0) {}

inline RedisConnection::RedisConnection(redisContext* context)
	: _context(context) {}

inline bool RedisConnection::expire(const char* key, time_t expire)
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

inline int RedisConnection::del(const char* key)
{
	std::vector<std::string> vec;
	vec.push_back(key);
	return del(vec);
}
inline int RedisConnection::del(const std::vector<std::string>& keys)
{
	if (keys.empty())
		return 0;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "DEL ";
	for (std::vector<std::string>::const_iterator iter = keys.begin();
		iter != keys.end(); ++iter)
		k += *iter + " ";

	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int size = 0;
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
		size = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return size;
}
inline int RedisConnection::del(const std::list<std::string>& keys)
{
	if (keys.empty())
		return 0;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "DEL ";
	for (std::list<std::string>::const_iterator iter = keys.begin();
		iter != keys.end(); ++iter)
		k += *iter + " ";

	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int size = 0;
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
		size = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return size;
}

#endif