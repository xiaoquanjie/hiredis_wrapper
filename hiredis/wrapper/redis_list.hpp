#ifndef M_REDIS_LIST_INCLUDE
#define M_REDIS_LIST_INCLUDE

template<typename T>
inline int RedisConnection::lpush(const char* key, const T& value)
{
	std::ostringstream oss;
	oss << value;
	return lpush(key, oss.str());
}
inline int RedisConnection::lpush(const char* key, const std::string& value)
{
	std::vector<std::string> vec;
	vec.push_back(value);
	return lpush(key, vec, (std::string*)0);
}
template<typename T>
int RedisConnection::lpush(const char* key, const T& values, typename T::value_type*)
{
	if (values.empty())
		return 0;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "LPUSH " + std::string(key) + " ";
	for (typename T::const_iterator iter = values.begin();
		iter != values.end(); ++iter) {
		std::ostringstream oss;
		oss << *iter;
		k += oss.str() + " ";
	}

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
template<typename T>
int RedisConnection::lpush(const char* key, const T& values, std::string*)
{
	if (values.empty())
		return 0;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "LPUSH " + std::string(key) + " ";
	for (typename T::const_iterator iter = values.begin();
		iter != values.end(); ++iter)
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


template<typename T>
void RedisConnection::lrange(const char* key, int beg_idx, int end_idx, T& values, typename T::value_type*)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "LRANGE " + std::string(key) + " %d %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), beg_idx, end_idx);
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
			redisReply* ele = reply->element[idx];
			std::istringstream iss(std::string(ele->str, ele->len));
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
void RedisConnection::lrange(const char* key, int beg_idx, int end_idx, T& values, std::string*)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "LRANGE " + std::string(key) + " %d %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), beg_idx, end_idx);
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
			redisReply* ele = reply->element[idx];
			values.push_back(std::string(ele->str, ele->len));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}


template<typename T>
inline bool RedisConnection::lpop(const char* key, T& value)
{
	std::string v;
	if (lpop(key, v)) {
		std::istringstream iss(v);
		iss >> value;
		return true;
	}
	return false;
}
inline bool RedisConnection::lpop(const char*key, std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "LPOP " + std::string(key);
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
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
			ret = false;
			break;
		}
		if (reply->type != REDIS_REPLY_STRING) {
			error = RedisException(M_ERR_NOT_DEFINED);
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
inline bool RedisConnection::lpop(const char*key, char* value, unsigned int len)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "LPOP " + std::string(key);
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
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
			ret = false;
			break;
		}
		if (reply->type != REDIS_REPLY_STRING) {
			error = RedisException(M_ERR_NOT_DEFINED);
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
inline int RedisConnection::rpush(const char* key, const T& value)
{
	std::ostringstream oss;
	oss << value;
	return rpush(key, oss.str());
}
inline int RedisConnection::rpush(const char* key, const std::string& value)
{
	std::vector<std::string> vec;
	vec.push_back(value);
	return rpush(key, vec, (std::string*)0);
}
template<typename T>
int RedisConnection::rpush(const char* key, const T& values, typename T::value_type*)
{
	if (values.empty())
		return 0;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "RPUSH " + std::string(key) + " ";
	for (typename T::const_iterator iter = values.begin();
		iter != values.end(); ++iter) {
		std::ostringstream oss;
		oss << *iter;
		k += oss.str() + " ";
	}

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
template<typename T>
int RedisConnection::rpush(const char* key, const T& values, std::string*)
{
	if (values.empty())
		return 0;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "RPUSH " + std::string(key) + " ";
	for (typename T::const_iterator iter = values.begin();
		iter != values.end(); ++iter)
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


template<typename T>
inline bool RedisConnection::rpop(const char* key, T& value)
{
	std::string v;
	if (rpop(key, v)) {
		std::istringstream iss(v);
		iss >> value;
		return true;
	}
	return false;
}
inline bool RedisConnection::rpop(const char*key, std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "RPOP " + std::string(key);
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
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
			ret = false;
			break;
		}
		if (reply->type != REDIS_REPLY_STRING) {
			error = RedisException(M_ERR_NOT_DEFINED);
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
inline bool RedisConnection::rpop(const char*key, char* value, unsigned int len)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "LPOP " + std::string(key);
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
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
			ret = false;
			break;
		}
		if (reply->type != REDIS_REPLY_STRING) {
			error = RedisException(M_ERR_NOT_DEFINED);
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

inline int RedisConnection::llen(const char* key)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "LLEN " + std::string(key);
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
inline bool RedisConnection::lindex(const char* key, int idx, T&value)
{
	std::string v;
	if (lindex(key, idx, v)) {
		std::istringstream iss(v);
		iss >> value;
		return true;
	}
	return false;
}
inline bool RedisConnection::lindex(const char* key, int idx, std::string&value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("LINDEX ") + std::string(key) + " %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), idx);
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
			ret = false;
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
inline bool RedisConnection::lindex(const char* key, int idx, char* value, int len)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("LINDEX ") + std::string(key) + " %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), idx);
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
			ret = false;
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
inline void RedisConnection::lset(const char* key, int idx, T value)
{
	std::ostringstream oss;
	oss << value;
	lset(key, idx, oss.str());
}
inline void RedisConnection::lset(const char* key, int idx, const std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("LSET ") + std::string(key) + " %d %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), idx, value.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	do 
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if (reply->type != REDIS_REPLY_STATUS) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		if (strcasecmp(reply->str, "OK") != 0) {
			error = RedisException(reply->str);
			break;
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
inline void RedisConnection::lset(const char* key, int idx, const char* value, unsigned int len)
{
	lset(key, idx, std::string(value, len));
}
template<int N>
inline void RedisConnection::lset(const char* key, int idx, const char(&value)[N])
{
	lset(key, idx, std::string(value, N));
}

template<typename T>
inline int RedisConnection::lrem(const char* key, int idx, T value)
{
	std::ostringstream oss;
	oss << value;

	M_CHECK_REDIS_CONTEXT(_context);
	std::string k = std::string("LREM ") + std::string(key) + " %d %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), idx, oss.str().c_str());
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

inline void RedisConnection::ltrim(const char*key, int beg_idx, int end_idx)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("LTRIM ") + std::string(key) + " %d %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), beg_idx, end_idx);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	do
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if (reply->type != REDIS_REPLY_STATUS) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		if (strcasecmp(reply->str, "OK") != 0) {
			error = RedisException(reply->str);
			break;
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}

template<typename T1, typename T2>
inline int RedisConnection::linsert(const char* key, bool b_or_a, const T1& value1, const T2& value2)
{
	std::ostringstream oss2;
	oss2 << value2;
	std::ostringstream oss1;
	oss1 << value1;
	return linsert(key, b_or_a, oss1.str(), oss2.str());
}

inline int RedisConnection::linsert(const char* key, bool b_or_a, const std::string& value1, const std::string&value2)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("LINSERT ") + std::string(key);
	if (b_or_a)
		k += std::string(" BEFORE ") + " %s %s";
	else
		k += std::string(" AFTER ") + " %s %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), value1.c_str(), value2.c_str());
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
inline bool RedisConnection::rpoplpush(const char*key1, const char*key2, T& value)
{
	std::string v;
	if (rpoplpush(key1,key2,v))
	{
		std::istringstream iss(v);
		v >> value;
		return true;
	}
	return false;
}
inline bool RedisConnection::rpoplpush(const char*key1, const char*key2, std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);
	
	redisReply* reply = (redisReply*)redisCommand(_context, "RPOPLPUSH %s %s",key1,key2);
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
			ret = false;
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

#endif