#ifndef __LOCK_HPP__
#define __LOCK_HPP__

#include <pthread.h>
#include <exception>
#include <semaphore.h>//信号量

namespace LOCk{


class sem
{
private:
	sem_t* s;//信号

public:
	sem()
	{
		if( sem_init(s,0,0) != 0)
        {
            throw std::exception();
        }
	}

	~sem()
    {
        sem_destroy(s);
    }

	bool wait()
    {
        return sem_wait(s) == 0;
    }

    bool post()
    {
        return sem_post(s) == 0;
    }
};

class locker
{
private:
    pthread_mutex_t* mutex;//互斥锁

public:
    locker()
    {
        if( pthread_mutex_init(mutex,NULL) != 0)
        {
            throw std::exception();
        }
    }

    ~locker()
    {
        pthread_mutex_destroy(mutex);
    }

    bool lock()
    {
        return pthread_mutex_lock(mutex) == 0;
    }

    bool unlock()
    {
        return pthread_mutex_unlock(mutex) == 0;
    }
};

class cond
{
private:
    pthread_mutex_t* mutex;
    pthread_cond_t* cond;//条件变量

public:
    cond()
    {
        if(pthread_mutex_init(mutex,NULL) != 0)
        {
            throw std::exception();
        }
        if (pthread_cond_init(cond,NULL) != 0)
        {
            pthread_mutex_destroy(mutex);
            throw std::exception();
        }
    }

    ~cond()
    {
        pthread_mutex_destroy(mutex);
        pthread_cond_destroy(cond);
    }

    bool wait()
    {
        int ret = 0;
        pthread_mutex_lock(mutex);
        ret = pthread_cond_wait(cond,mutex);
        pthread_mutex_unlock(mutex);
        return ret == 0;
    }

    bool signal()
    {
        return pthread_cond_signal(cond) == 0;
    }
};

}
#endif
