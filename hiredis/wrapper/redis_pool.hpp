#ifndef M_REDIS_POOL_INCLUDE
#define M_REDIS_POOL_INCLUDE

#include <arpa/inet.h>
#include "mutex.hpp"
#include <set>

// Note: A redisContext is not thread - safe.
// netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'

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

struct _redisinfoset_ {
	std::set<_rediscontext_*> _contexts;

	~_redisinfoset_() {
		for (std::set<_rediscontext_*>::iterator iter = _contexts.begin();
			iter != _contexts.end(); ++iter) {
			redisFree((*iter)->_context);
			delete ((_rediscontext_*)*iter);
		}
		_contexts.clear();
	}
};

// redis连接池
class RedisPool
{
private:
	static _redisinfoset_ _contexts;
	static _redis_detail::MutexLock _mutex;

public:
	// 每个线程都会有一个连接,最好别跨线程使用,否则不保证线程安全
	static RedisConnection GetConnection(const std::string& ip, unsigned short port);

	// 每个线程都会有一个连接,最好别跨线程使用,否则不保证线程安全
	static RedisConnection GetConnection(const std::string& ip, unsigned short port, unsigned short database);

	// 释放连接
	static void ReleaseConnection(RedisConnection& con);

	static size_t GetConnectionCnt();

private:
	static void _releaseConnection(_rediscontext_* context);

	static bool _selectdb(redisContext* context, unsigned short db);

	static inline void _freeRedisContext(redisContext* context);
};

_redisinfoset_ RedisPool::_contexts;
_redis_detail::MutexLock RedisPool::_mutex;

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
		_rediscontext_* _context_ = (_rediscontext_*)iter->second;
		_context_->_ref++;
		return RedisConnection(_context_);
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

		_rediscontext_* _context_ = new _rediscontext_;
		_context_->_context = context;
		_context_->_ip = ip;
		_context_->_port = port;
		_context_->_db = database;
		_context_->_ref = 2;
		pinfo->_info[unique_id] = (void*)_context_;
		pinfo->_revinfo[_context_] = unique_id;
		{
			_redis_detail::ScopedLock scoped_l(_mutex);
			_contexts._contexts.insert(_context_);
		}
		return RedisConnection(_context_);
	}
}

bool RedisPool::_selectdb(redisContext* context, unsigned short db)
{
	if (!context)
		return false;

	redisReply* reply = (redisReply*)redisCommand(context,"select %d",db);
	if (!reply) {
		throw RedisException(context->errstr);
	}

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

void RedisPool::_releaseConnection(_rediscontext_* context)
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
		{
			_redis_detail::ScopedLock scoped_l(_mutex);
			_contexts._contexts.erase(context);
		}
	} while (false);
	_freeRedisContext(context->_context);
	context->_context = 0;
}

void RedisPool::ReleaseConnection(RedisConnection& con)
{
	_releaseConnection(con._context);
	con._context->_ref--;
	if (con._context->_ref <= 0) {
		delete con._context;
		con._context = 0;
	}
}

void RedisPool::_freeRedisContext(redisContext* context)
{
	redisFree(context);
}

size_t RedisPool::GetConnectionCnt() {
	return _contexts._contexts.size();
}

#endif