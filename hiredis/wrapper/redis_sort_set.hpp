#ifndef M_REDIS_SORT_SET_INCLUDE
#define M_REDIS_SORT_SET_INCLUDE

template<typename T1, typename T2>
int RedisConnection::zadd(const char*key, const T1& score, const T2& member)
{
	std::ostringstream oss;
	oss << member;
	return zadd(key, score, oss.str());
}
template<typename T>
int RedisConnection::zadd(const char*key, const T& score, const std::string& member)
{
	M_CHECK_REDIS_CONTEXT(_context);
	
	std::ostringstream oss;
	oss << score;
	redisReply* reply = (redisReply*)redisCommand(_context, "ZADD %s %s %s", key,oss.str().c_str(),member.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int ret = 0;
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
		ret = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}

template<typename T>
void RedisConnection::zrange(const char*key, int beg_idx, int end_idx, T& values, typename T::value_type*)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "ZRANGE %s %d %d", key, beg_idx, end_idx);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	do
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if (reply->type != REDIS_REPLY_ARRAY) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		for (size_t idx = 0; idx < reply->elements; ++idx) {
			std::istringstream iss(std::string(reply->element[idx]->str, reply->element[idx]->len));
			typename T::value_type v;
			iss >> v;
			values.push_back(v);
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
template<typename T>
void RedisConnection::zrange(const char*key, int beg_idx, int end_idx, T& values, std::string*)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "ZRANGE %s %d %d", key, beg_idx,end_idx);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	do 
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if (reply->type != REDIS_REPLY_ARRAY) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		for (size_t idx=0; idx<reply->elements; ++idx){
			values.push_back(std::string(reply->element[idx]->str, reply->element[idx]->len));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}

template<typename T1, typename T2, typename T3>
void RedisConnection::zrangewithscores(const char* key, int beg_idx, int end_idx, T1& values,
	std::pair<T2, T3>*)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "ZRANGE %s %d %d withscores", key, beg_idx, end_idx);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	do 
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if (reply->type == REDIS_REPLY_ARRAY) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		for (size_t idx = 0; idx < reply->elements; idx+=2) {
			std::istringstream iss1(std::string(reply->element[idx]->str, reply->element[idx]->len));
			std::istringstream iss2(std::string(reply->element[idx+1]->str, reply->element[idx+1]->len));
			T3 member;
			iss1 >> member;
			T2 score;
			iss2 >> score;
			values.push_back(std::make_pair(score, member));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
template<typename T1, typename T2>
void RedisConnection::zrangewithscores(const char* key, int beg_idx, int end_idx, std::map<T1, T2>& values)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "ZRANGE %s %d %d withscores", key, beg_idx, end_idx);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	do
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if (reply->type == REDIS_REPLY_ARRAY) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		for (size_t idx = 0; idx < reply->elements; idx += 2) {
			std::istringstream iss1(std::string(reply->element[idx]->str, reply->element[idx]->len));
			std::istringstream iss2(std::string(reply->element[idx + 1]->str, reply->element[idx + 1]->len));
			T2 member;
			iss1 >> member;
			T1 score;
			iss2 >> score;
			values.insert(std::make_pair(score, member));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}

template<typename T>
bool RedisConnection::zrem(const char*key, const T& member)
{
	std::ostringstream oss;
	oss << member;
	return zrem(key, oss.str());
}
bool RedisConnection::zrem(const char*key, const std::string& member)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "ZREM %s %s", key,member.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	bool ret = false;
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
		ret = (bool)reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}
template<typename T>
int  RedisConnection::zrem(const char*key, const T& members, typename T::value_type*)
{
	M_CHECK_REDIS_CONTEXT(_context);
	std::string k = "ZREM " + std::string(key) + " ";
	for (typename T::const_iterator iter = members.begin(); iter != members.end(); ++iter) {
		std::ostringstream oss;
		oss << *iter;
		k += oss.str() + " ";
	}
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int ret = false;
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
		ret = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}
template<typename T>
int  RedisConnection::zrem(const char*key, const T& members, std::string*)
{
	M_CHECK_REDIS_CONTEXT(_context);
	std::string k = "ZREM " + std::string(key) + " ";
	for (typename T::const_iterator iter = members.begin(); iter != members.end(); ++iter)
		k += *iter + " ";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int ret = false;
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
		ret = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}


#endif