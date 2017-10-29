#ifndef M_REDIS_STRING_INCLUDE
#define M_REDIS_STRING_INCLUDE

template<typename T>
inline void RedisConnection::SetCommand(const char* key, T value)
{
	std::ostringstream oss;
	oss << value;
	SetCommand(key, oss.str());
}
inline void RedisConnection::SetCommand(const char* key, const std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("SET ") + std::string(key) + " %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(),value.c_str());
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
inline void RedisConnection::SetCommand(const char* key, const char* value, unsigned int len)
{
	SetCommand(key, std::string(value, len));
}
template<int N>
inline void RedisConnection::SetCommand(const char* key, const char(&value)[N])
{
	SetCommand(key, std::string(value, N));
}

template<typename T>
inline void RedisConnection::GetCommand(const char* key, T& value)
{
	std::string v;
	GetCommand(key, v);
	std::istringstream iss(v);
	iss >> value;
}
inline void RedisConnection::GetCommand(const char* key, std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("GET ") + key;
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
		if (reply->type == REDIS_REPLY_NIL) {
			error = RedisException(M_ERR_REDIS_KEY_NOT_EXIST);
			break;
		}
		
		value.clear();
		value.append(reply->str,reply->len);
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
inline void RedisConnection::GetCommand(const char* key, char* value, unsigned int len)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("GET ") + key;
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


inline void RedisConnection::IncrbyCommand(const char* key, int step)
{
	int value = 0;
	IncrbyCommand(key, step, value);
}
template<typename T>
inline void RedisConnection::IncrbyCommand(const char* key, int step, T& new_value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "INCRBY " + std::string(key) + " %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), step);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

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
		new_value = (T)reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
inline void RedisConnection::IncrCommand(const char* key)
{
	IncrbyCommand(key, 1);
}
template<typename T>
inline void RedisConnection::IncrCommand(const char* key, T& new_value)
{
	IncrbyCommand(key, 1, new_value);
}


template<typename T>
inline void RedisConnection::DecrbyCommand(const char* key, int step, T& new_value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "DECRBY " + std::string(key) + " %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), step);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

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
		new_value = (T)reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}
inline void RedisConnection::DecrbyCommand(const char* key, int step)
{
	int value = 0;
	DecrbyCommand(key, step, value);
}
template<typename T>
inline void RedisConnection::DecrCommand(const char* key, T& new_value)
{
	DecrbyCommand(key, 1, new_value);
}
inline void RedisConnection::DecrCommand(const char* key)
{
	DecrbyCommand(key, 1);
}


inline int RedisConnection::StrlenCommand(const char* key)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "STRLEN " + std::string(key);
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
		len = (int)reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return len;
}
inline int RedisConnection::AppendCommand(const char* key, const std::string& app_value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "APPEND " + std::string(key) + " %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(),app_value.c_str());
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
		len = (int)reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return len;
}
inline int RedisConnection::AppendCommand(const char* key, const char* value, unsigned int len)
{
	return AppendCommand(key, std::string(value, len));
}


template<typename T>
inline bool RedisConnection::SetnxCommand(const char* key, T value)
{
	std::ostringstream oss;
	oss << value;
	return SetnxCommand(key, oss.str());
}
inline bool RedisConnection::SetnxCommand(const char* key, const std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "SETNX " + std::string(key) + " %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(),value.c_str());
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	RedisException error;
	bool flag = false;
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
		flag = (bool)reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return flag;
}
inline bool RedisConnection::SetnxCommand(const char* key, const char* value, unsigned int len)
{
	return SetnxCommand(key, std::string(value, len));
}
template<int N>
inline bool RedisConnection::SetnxCommand(const char* key, const char(&value)[N])
{
	return SetnxCommand(key, std::string(value, N));
}


template<typename T>
inline void RedisConnection::SetexCommand(const char* key, T value, time_t expire)
{
	std::ostringstream oss;
	oss << value;
	SetexCommand(key, oss.str(), expire);
}
inline void RedisConnection::SetexCommand(const char* key, const std::string& value, time_t expire)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "SETEX " + std::string(key) + " %d %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(),expire,value.c_str());
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
inline void RedisConnection::SetexCommand(const char* key, const char* value, unsigned int len, time_t expire)
{
	SetexCommand(key, std::string(value, len), expire);
}
template<int N>
inline void RedisConnection::SetexCommand(const char* key, const char(&value)[N], time_t expire)
{
	SetexCommand(key, std::string(value, N), expire);
}

inline void RedisConnection::GetRangeCommand(const char* key, int beg_idx, int end_idx, std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("GETRANGE ") + std::string(key) + " %d %d";
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
		if (reply->type != REDIS_REPLY_STRING) {
			error = RedisException(M_ERR_NOT_DEFINED);
			break;
		}
		value.clear();
		value.append(reply->str, reply->len);
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;
}

inline int RedisConnection::SetRangeCommand(const char* key, int beg_idx, const char* value, unsigned int len)
{
	return SetRangeCommand(key, beg_idx, std::string(value, len));
}
inline int RedisConnection::SetRangeCommand(const char* key, int beg_idx, const std::string& value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = std::string("SETRANGE ") + std::string(key) + " %d %s";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(),beg_idx,value.c_str());
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
template<int N>
inline int RedisConnection::SetRangeCommand(const char* key, int beg_idx, const char(&value)[N])
{
	return SetRangeCommand(key, beg_idx, std::string(value, N));
}

inline int RedisConnection::SetbitCommand(const char* key, unsigned int offset, int value)
{
	M_CHECK_REDIS_CONTEXT(_context);

	if (value != 0)
		value = 1;
	std::string k = "SETBIT " + std::string(key) + " %d %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), offset, value);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	value = 0;
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
		value = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return value;
}
inline int RedisConnection::GetbitCommand(const char* key, unsigned int offset)
{
	M_CHECK_REDIS_CONTEXT(_context);

	std::string k = "GETBIT " + std::string(key) + " %d";
	redisReply* reply = (redisReply*)redisCommand(_context, k.c_str(), offset);
	if (!reply)
		throw RedisException(M_ERR_REDIS_REPLY_NULL);

	int value = 0;
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
		value = reply->integer;
	} while (false);

	freeReplyObject(reply);
	if (!error.Empty())
		throw error;

	return value;
}

#endif