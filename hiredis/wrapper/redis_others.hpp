#ifndef M_REDIS_OTHERS_INCLUDE
#define M_REDIS_OTHERS_INCLUDE

inline RedisConnection::RedisConnection()
	:_context(0){}

inline RedisConnection::RedisConnection(_rediscontext_* context)
	: _context(context){}

inline RedisConnection::RedisConnection(const RedisConnection& conn) {
	if (conn._context)
		conn._context->_ref++;
	this->_context = conn._context;
}

inline RedisConnection::~RedisConnection(){
	if (_context) {
		_context->_ref--;
		if (_context->_ref <= 0)
			delete _context;
	}
}

inline RedisConnection& RedisConnection::operator=(const RedisConnection& conn) {
	if (_context != conn._context) {
		if (_context) {
			_context->_ref--;
			if (_context->_ref <= 0)
				delete _context;
		}
		if (conn._context)
			conn._context->_ref++;
		_context = conn._context;
	}
	return *this;
}

inline unsigned long long RedisConnection::connectionid()const {
	unsigned long long v = (unsigned long long)(_context);
	return v;
}

inline unsigned int RedisConnection::getrefcnt()const {
	if (!_context)
		return 0;
	return _context->_ref;
}

inline bool RedisConnection::expire(const char* key, time_t expire)
{
	M_CHECK_REDIS_CONTEXT(_context);
	std::string k = "EXPIRE " + std::string(key) + " %d";
	redisReply* reply = (redisReply*)redisCommand(M_REDIS_CONTEXT(_context), k.c_str(), expire);
	if (!reply)
		M_CLOSE_CONNECTION(this);
	
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
template<typename T>
int RedisConnection::del(const T& keys)
{
	if (keys.empty())
		return 0;

	M_CHECK_REDIS_CONTEXT(_context);
	std::string k = "DEL ";
	for (typename T::const_iterator iter = keys.begin();
		iter != keys.end(); ++iter) {
		k += *iter + " ";
	}

	redisReply* reply = (redisReply*)redisCommand(M_REDIS_CONTEXT(_context), k.c_str());
	if (!reply)
		M_CLOSE_CONNECTION(this);

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