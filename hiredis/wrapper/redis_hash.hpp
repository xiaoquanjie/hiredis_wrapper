#ifndef M_REDIS_HASH_INCLUDE
#define M_REDIS_HASH_INCLUDE

template<typename T>
inline int RedisConnection::hset(const char* key, const char* field, const T& value)
{
	std::ostringstream oss;
	oss << value;
	return hset(key, field, oss.str());
}
inline int RedisConnection::hset(const char* key, const char* field, const std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HSET " + std::string(key) + " %s %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), field, value.c_str());
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
inline int RedisConnection::hset(const char* key, const char* field, const char* value, unsigned int len)
{
	return hset(key, field, std::string(value, len));
}
template<int N>
inline int RedisConnection::hset(const char* key, const char* field, const char(&value)[N])
{
	return hset(key, field, std::string(value, N));
}

template<typename T>
inline void RedisConnection::hget(const char* key, const char* field, T& value)
{
	std::string v;
	hget(key, field, v);
	std::istringstream iss(v);
	iss >> value;
}
inline void RedisConnection::hget(const char* key, const char* field, std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HGET " + std::string(key) + " %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), field);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	do 
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if (reply->type == REDIS_REPLY_NIL) {
			error = RedisException(M_ERR_REDIS_KEY_NOT_EXIST);
			break;
		}
		value.clear();
		value.append(reply->str, reply->len);
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
inline void RedisConnection::hget(const char* key, const char* field, char* value, unsigned int len)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HGET " + std::string(key) + " %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), field);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	do
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if (reply->type == REDIS_REPLY_NIL) {
			error = RedisException(M_ERR_REDIS_KEY_NOT_EXIST);
			break;
		}
		if (len > reply->len)
			len = reply->len;
		memcpy(value, reply->str, len);
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}


template<typename T1, typename T2>
inline void RedisConnection::hgetall(const char* key, std::map<T1, T2>& values)
{
	values.clear();
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HGETALL %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(),key);
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
		for (size_t idx=0; idx<reply->elements; idx+=2)
		{
			std::istringstream iss1(std::string(reply->element[idx]->str, reply->element[idx]->len));
			std::istringstream iss2(std::string(reply->element[idx+1]->str, reply->element[idx+1]->len));
		
			T1 value1;
			iss1 >> value1;
			T2 value2;
			iss2 >> value2;
			values.insert(std::make_pair(value1, value2));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
inline void RedisConnection::hgetall(const char* key, std::map<std::string, std::string>& values)
{
	values.clear();
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HGETALL %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key);
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
		for (size_t idx = 0; idx < reply->elements; idx += 2)
		{
			values.insert(std::make_pair(std::string(reply->element[idx]->str, reply->element[idx]->len),
				std::string(reply->element[idx + 1]->str, reply->element[idx + 1]->len)));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
template<typename T>
inline void RedisConnection::hgetall(const char* key, std::map<T, std::string>& values)
{
	values.clear();
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HGETALL %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key);
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
		for (size_t idx = 0; idx < reply->elements; idx += 2)
		{
			std::istringstream iss1(std::string(reply->element[idx]->str, reply->element[idx]->len));
			T value1;
			iss1 >> value1;
			values.insert(std::make_pair(value1, 
				std::string(reply->element[idx + 1]->str, reply->element[idx + 1]->len)));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
template<typename T>
inline void RedisConnection::hgetall(const char* key, std::map<std::string, T>& values)
{
	values.clear();
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HGETALL %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key);
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
		for (size_t idx = 0; idx < reply->elements; idx += 2)
		{
			std::istringstream iss1(std::string(reply->element[idx + 1]->str, reply->element[idx + 1]->len));
			T value1;
			iss1 >> value1;
			values.insert(std::make_pair(std::string(reply->element[idx]->str, reply->element[idx]->len), value1));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}


template<typename T>
inline bool RedisConnection::hsetnx(const char* key, const char* field, const T& value)
{
	std::ostringstream oss;
	oss << value;
	return hsetnx(key, field, oss.str());
}
inline bool RedisConnection::hsetnx(const char* key, const char* field, const std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HSETNX " + std::string(key) + " %s %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), field, value.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	bool ret = 0;
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
inline bool RedisConnection::hsetnx(const char* key, const char* field, const char* value, unsigned int len)
{
	return hsetnx(key, field, std::string(value, len));
}
template<int N>
inline bool RedisConnection::hsetnx(const char* key, const char* field, const char(&value)[N])
{
	return hsetnx(key, field, std::string(value, N));
}


template<typename T1, typename T2>
inline void RedisConnection::hmset(const char* key, const std::map<T1, T2>& values)
{
	if (values.empty())
		return;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HMSET " + std::string(key) + " ";
	for (typename std::map<T1, T2>::const_iterator iter = values.begin();
		iter != values.end(); ++iter) {
		std::ostringstream oss1;
		oss1 << iter->first;
		std::ostringstream oss2;
		oss2 << iter->second;
		k += oss1.str() + " " + oss2.str() + " ";
	}

	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
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
inline void RedisConnection::hmset(const char* key, const std::map<std::string, std::string>& values)
{
	if (values.empty())
		return;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HMSET " + std::string(key) + " ";
	for (std::map<std::string, std::string>::const_iterator iter=values.begin(); 
		iter!=values.end(); ++iter){
		k += iter->first + " " + iter->second + " ";
	}

	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
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
template<typename T>
inline void RedisConnection::hmset(const char* key, const std::map<T, std::string>& values)
{
	if (values.empty())
		return;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HMSET " + std::string(key) + " ";
	for (typename std::map<T, std::string>::const_iterator iter = values.begin();
		iter != values.end(); ++iter) {
		std::ostringstream oss;
		oss << iter->first;
		k += oss.str() + " " + iter->second + " ";
	}

	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
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
template<typename T>
inline void RedisConnection::hmset(const char* key, const std::map<std::string, T>& values)
{
	if (values.empty())
		return;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HMSET " + std::string(key) + " ";
	for (typename std::map<std::string, T>::const_iterator iter = values.begin();
		iter != values.end(); ++iter) {
		std::ostringstream oss;
		oss << iter->second;
		k += iter->first + " " + oss.str() + " ";
	}

	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
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
inline void RedisConnection::hmget(const char* key, std::vector<TriangleValule<T1, T2> >&values)
{
	if (values.empty())
		return;
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HMGET " + std::string(key) + " ";
	for (typename std::vector<TriangleValule<T1, T2> >::iterator iter = values.begin();
		iter != values.end(); ++iter) {
		k += iter->Key() + " ";
	}

	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str());
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
		if (reply->elements != values.size()) {
			error = RedisException(M_ERR_REDIS_ARRAY_SIZE_NOT_MATCH);
			break;
		}
		for (size_t idx=0;idx<reply->elements; ++idx)
		{
			redisReply* rep = reply->element[idx];
			if (rep->type == REDIS_REPLY_NIL)
				values[idx]._valid = false;
			else {
				values[idx]._valid = true;
				values[idx].SetValue(rep->str, rep->len);
			}
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}


template<typename T>
inline void RedisConnection::hincrby(const char* key, const char* field, int step, T& new_value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HINCRBY %s %s %d ";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(),key,field,step);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	do 
	{
		if (reply->type == REDIS_REPLY_ERROR) {
			error = RedisException(reply->str);
			break;
		}
		if(reply->type != REDIS_REPLY_INTEGER) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		new_value = (T)reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
inline void RedisConnection::hincrby(const char* key, const char* field, int step)
{
	int value = 0;
	hincrby(key, field, step, value);
}


bool RedisConnection::hexists(const char* key, const char*field)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HEXISTS %s %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key, field);
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
int RedisConnection::hlen(const char* key)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HLEN %s ";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key);
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
		ret = (int)reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return ret;
}
bool RedisConnection::hdel(const char* key, const char*field)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HDEL %s %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key, field);
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
bool RedisConnection::hdel(const char*key, const T& field)
{
	std::ostringstream oss;
	oss << field;
	return hdel(key, oss.str().c_str());
}

template<typename T>
void RedisConnection::hkeys(const char*key, T& values, typename T::value_type*)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HKEYS %s ";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key);
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
void RedisConnection::hkeys(const char*key, T& values, std::string*)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HKEYS %s ";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(),key);
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
			values.push_back(std::string(reply->element[idx]->str, reply->element[idx]->len));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}


template<typename T>
void RedisConnection::hvals(const char*key, T& values, typename T::value_type*)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HVALS %s ";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key);
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
void RedisConnection::hvals(const char*key, T& values, std::string*)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "HVALS %s ";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), key);
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
			values.push_back(std::string(reply->element[idx]->str, reply->element[idx]->len));
		}
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}

#endif