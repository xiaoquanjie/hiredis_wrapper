#ifndef M_REDIS_SET_INCLUDE
#define M_REDIS_SET_INCLUDE

template<typename T>
int RedisConnection::sadd(const char*key, const T& value)
{
	std::ostringstream oss;
	oss << value;
	return sadd(key, oss.str());
}
int RedisConnection::sadd(const char*key, const std::string&value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "SADD %s %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key, value.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int len = 0;
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
		len = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return len;
}
template<typename T>
int RedisConnection::sadd(const char*key, const T& values, typename T::value_type)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "SADD %s " + std::string(key);
	for (typename T::const_iterator iter = values.begin();
		iter != values.end(); ++iter) {
		std::ostringstream oss;
		oss << *iter;
		k += oss.str() + " ";
	}

	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int len = 0;
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
		len = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return len;
}
template<typename T>
int RedisConnection::sadd(const char*key, const T& values, std::string)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "SADD %s " + std::string(key);
	for (typename T::const_iterator iter=values.begin();
		iter!=values.end(); ++iter)
		k += *iter + " ";

	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int len = 0;
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
		len = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return len;
}



#endif