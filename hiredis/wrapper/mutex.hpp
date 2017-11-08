/*----------------------------------------------------------------
// Copyright (C) 2017 public
//
// 修改人：xiaoquanjie
// 时间：2017/8/10
//
// 修改人：xiaoquanjie
// 时间：
// 修改说明：
//
// 版本：V1.0.0
//----------------------------------------------------------------*/

#ifndef M_MUTEXLOCK_INCLUDE
#define M_MUTEXLOCK_INCLUDE

#include <pthread.h>

namespace _redis_detail {

	class MutexLock
	{
	public:
		MutexLock()
		{
			pthread_mutex_init(&_mutex, 0);
		}
		~MutexLock()
		{
			pthread_mutex_destroy(&_mutex);
		}
		void lock()
		{
			pthread_mutex_lock(&_mutex);
		}
		void unlock()
		{
			pthread_mutex_unlock(&_mutex);
		}

	private:
		MutexLock(const MutexLock&);
		MutexLock& operator=(const MutexLock&);

		pthread_mutex_t _mutex;
	};

	class ScopedLock
	{
	public:
		ScopedLock(MutexLock& mutex) :_mutex(mutex)
		{
			_mutex.lock();
		}
		~ScopedLock()
		{
			_mutex.unlock();
		}
	private:
		ScopedLock(const ScopedLock&);
		ScopedLock& operator=(const ScopedLock&);

		MutexLock& _mutex;
	};

}
#endif