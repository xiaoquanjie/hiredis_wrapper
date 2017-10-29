#ifndef M_REDIS_WRAPPER_CONFIG_INCLUDE
#define M_REDIS_WRAPPER_CONFIG_INCLUDE

#ifndef M_PLATFORM_WIN32
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define M_PLATFORM_WIN32 1
#endif
#endif

#ifndef M_PLATFORM_WIN
#if defined(M_PLATFORM_WIN32) || defined(WIN64)
#define M_PLATFORM_WIN 1
#endif
#endif

#include "../hiredis.h"
#include <string>
#include <string.h>
#include <vector>
#include <list>

#ifdef M_PLATFORM_WIN
#include <memory>
#else
#include <tr1/memory>
#endif

#define shard_ptr_t std::tr1::shared_ptr

// redis“Ï≥£
struct RedisException
{
	RedisException() {}

	RedisException(const char* what) {
		_what.reset(new std::string(what));
	}

	RedisException(const std::string& what) {
		_what.reset(new std::string(what));
	}

	std::string What()const {
		if (!_what)
			return std::string();
		return *_what;
	}

	bool Empty()const {
		return (!_what);
	}

private:
	shard_ptr_t<std::string> _what;
};

#define M_ERR_NOT_DEFINED			("not defined error")
#define M_ERR_REDIS_CONNECT_FAIL	("redisConnect fail")
#define M_ERR_REDIS_NOT_CONNECTED	("redis is not connected")
#define M_ERR_REDIS_REPLY_NULL		("redis reply is null")
#define M_ERR_REDIS_TYPE_NOT_MATCH	("type doesn't match")
#define M_ERR_REDIS_KEY_NOT_EXIST	("key not exist")

std::string gTypeStr[REDIS_REPLY_ERROR + 1] = {
	" not defined",
	" string",
	" array",
	" integer",
	" nil",
	" status",
	" error"
};

#endif