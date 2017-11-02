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
int RedisConnection::sadd(const char*key, const T& values, typename T::value_type*)
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
int RedisConnection::sadd(const char*key, const T& values, std::string*)
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


// T要求是一个容器
template<typename T>
void RedisConnection::smembers(const char*key, T&values, typename T::value_type*)
{
	M_CHECK_REDIS_CONTEXT(_context);

	redisReply* reply = (redisReply*)redisCommand(_context, "SMEMBERS %s", key);
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
		for (size_t idx = 0; idx < reply->elements; ++idx){
			typename T::value_type v;
			std::istringstream iss(std::string(reply->element[idx]->str, reply->element[idx]->len));
			iss >> v;
			values.push_back(v);
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
template<typename T>
void RedisConnection::smembers(const char*key, T&values, std::string*)
{
	M_CHECK_REDIS_CONTEXT(_context);

	redisReply* reply = (redisReply*)redisCommand(_context, "SMEMBERS %s", key);
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


template<typename T>
bool RedisConnection::spop(const char*key, T&value)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SPOP %s", key);
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
		if (reply->type == REDIS_REPLY_NIL) {
			break;
		}
		std::istringstream iss(std::string(reply->str, reply->len));
		iss >> value;
		ret = true;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}
bool RedisConnection::spop(const char*key, std::string&value)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SPOP %s", key);
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
		if (reply->type == REDIS_REPLY_NIL) {
			break;
		}

		value.clear();
		value.append(reply->str, reply->len);
		ret = true;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}
bool RedisConnection::spop(const char*key, char*value, unsigned int len)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SPOP %s", key);
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
		if (reply->type == REDIS_REPLY_NIL) {
			break;
		}
		if (len > reply->len)
			len = reply->len;
		memcpy(value, reply->str, len);
		ret = true;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}

template<typename T>
bool RedisConnection::srem(const char*key, const T& field)
{
	std::ostringstream oss;
	oss << field;
	return srem(key, oss.str().c_str());
}
bool RedisConnection::srem(const char*key, const char* field)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SREM %s %s", key, field);
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


int RedisConnection::scard(const char*key)
{
	M_CHECK_REDIS_CONTEXT(_context);

	redisReply* reply = (redisReply*)redisCommand(_context, "SCARD %s", key);
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
bool RedisConnection::sismember(const char*key, const char* field)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SISMEMBER %s",key);
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
bool RedisConnection::somve(const char* src_key, const char* dst_key)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SMOVE %s %s", src_key,dst_key);
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
bool RedisConnection::srandmember(const char*key, T&value)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SRANDMEMBER %s", key);
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
		if (reply->type == REDIS_REPLY_NIL) {
			break;
		}
		std::istringstream iss(std::string(reply->str, reply->len));
		iss >> value;
		ret = true;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}
bool RedisConnection::srandmember(const char*key, std::string&value)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SRANDMEMBER %s", key);
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
		if (reply->type == REDIS_REPLY_NIL) {
			break;
		}
		value.clear();
		value.append(reply->str, reply->len);
		ret = true;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}
template<typename T>
void RedisConnection::srandmember(const char*key, T&values, int count, typename T::value_type*)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SRANDMEMBER %s %d", key,count);
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
			std::istringstream iss(std::string(reply->element[idx]->str , reply->element[idx]->len));
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
void RedisConnection::srandmember(const char*key, T&values, int count, std::string*)
{
	M_CHECK_REDIS_CONTEXT(_context);
	redisReply* reply = (redisReply*)redisCommand(_context, "SRANDMEMBER %s %d", key, count);
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
			std::string v(reply->element[idx]->str, reply->element[idx]->len);
			values.push_back(v);
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}

#endif