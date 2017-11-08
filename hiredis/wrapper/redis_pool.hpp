#ifndef M_REDIS_POOL_INCLUDE
#define M_REDIS_POOL_INCLUDE

#include <arpa/inet.h>
#include "mutex.hpp"
#include <set>

// Note: A redisContext is not thread - safe.

// 线程局部对象
template<typename T>
struct ThreadLocalData {
	static __thread T* _pt;
};
template<typename T>
__thread T* ThreadLocalData<T>::_pt = 0;

// redis地址
struct _redisaddr_ {
	unsigned int _ip;
	unsigned short _port;
	unsigned short _db;

	unsigned long long two_only_id()const {
		unsigned long long v = ((unsigned long long)_ip << 32);
		v += ((unsigned long long)_port << 16);
		return v;
	}
	unsigned long long three_only_id()const {
		unsigned long long v = ((unsigned long long)_ip << 32);
		v += ((unsigned long long)_port << 16);
		v += _db;
		return v;
	}
	static unsigned long long two_only_id(unsigned int ip, unsigned short port){
		unsigned long long v = ((unsigned long long)ip << 32);
		v += ((unsigned long long)port << 16);
		return v;
	}
	static unsigned long long three_only_id(unsigned int ip, unsigned short port,unsigned short db){
		unsigned long long v = ((unsigned long long)ip << 32);
		v += ((unsigned long long)port << 16);
		v += db;
		return v;
	}
};

struct _redisinfo_ {
	std::map<unsigned long long, void*> _info;
	std::map<void*, unsigned long long> _revinfo;
};

struct _rediscontext {
	redisContext* _context;
};

// redis连接池
class RedisPool
{
private:
	static std::set<redisContext*> _context_set;
public:
	// 每个线程都会有一个连接,最好别跨线程使用,否则不保证线程安全
	static RedisConnection GetConnection(const std::string& ip, unsigned short port);

	// 每个线程都会有一个连接,最好别跨线程使用,否则不保证线程安全
	static RedisConnection GetConnection(const std::string& ip, unsigned short port, unsigned short database);

	// 释放连接
	static void ReleaseConnection(redisContext* context);
	static void ReleaseConnection(RedisConnection& con);

private:
	static bool _selectdb(redisContext* context, unsigned short db);

	static inline void _freeRedisContext(redisContext* context);
};

std::set<redisContext*> RedisPool::_context_set;

RedisConnection RedisPool::GetConnection(const std::string& ip, unsigned short port)
{
	return GetConnection(ip, port, 0);
}

RedisConnection RedisPool::GetConnection(const std::string& ip, unsigned short port, unsigned short database)
{
	typedef ThreadLocalData<int> RedisDataType;
	if (!RedisDataType::_pt) {
		RedisDataType::_pt = (int*)(new _redisinfo_);
	}

	_redisinfo_* pinfo = (_redisinfo_*)RedisDataType::_pt;
	unsigned long long unique_id = _redisaddr_::three_only_id((unsigned int)inet_addr(ip.c_str()),
		port, database);

	std::map<unsigned long long, void*>::iterator iter = pinfo->_info.find(unique_id);
	if (iter!=pinfo->_info.end()) {
		return RedisConnection((redisContext*)iter->second);
	}
	else {
		redisContext* context = redisConnect(ip.c_str(), port);
		try {
			if (!context)
				throw RedisException(M_ERR_REDIS_CONNECT_FAIL);
			if (context->err != 0)
				throw RedisException(context->errstr);
			if (database != 0 && !_selectdb(context, database))
				throw RedisException(M_ERR_REDIS_SELECT_DB_ERROR);
		}
		catch (RedisException e) {
			_freeRedisContext(context);
			throw e;
		}

		pinfo->_info[unique_id] = (void*)context;
		pinfo->_revinfo[context] = unique_id;
		return RedisConnection(context);
	}
}

bool RedisPool::_selectdb(redisContext* context, unsigned short db)
{
	if (!context)
		return false;

	redisReply* reply = (redisReply*)redisCommand(context,"select %d",db);
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
	if (!error.Empty()) {
		throw error;
	}
	return true;
}

void RedisPool::ReleaseConnection(redisContext* context)
{
	do 
	{
		typedef ThreadLocalData<int> RedisDataType;
		if (!RedisDataType::_pt)
			break;

		_redisinfo_* pinfo = (_redisinfo_*)RedisDataType::_pt;
		std::map<void*, unsigned long long>::iterator iter = pinfo->_revinfo.find(context);
		if (iter==pinfo->_revinfo.end())
			break;

		pinfo->_info.erase(iter->second);
		pinfo->_revinfo.erase(iter);
	} while (false);
	_freeRedisContext(context);
}

void RedisPool::ReleaseConnection(RedisConnection& con)
{
	ReleaseConnection(con._context);
	con._context = 0;
}

void RedisPool::_freeRedisContext(redisContext* context)
{
	redisFree(context);
}

#endif