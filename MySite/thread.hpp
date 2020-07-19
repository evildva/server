#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>
#include <exception>
#include <functional>
#include <csignal>

namespace THREAd
{
typedef void* (*handler)(void* arg);

class thread
{
	private:
		pthread_t* thd;
		void* ret;

	private:
		handler start;
		static void* hand(void* arg)
		{
			(*start)(arg);
			//pthread_exit(ret);
		}
		

	public:
		thread(handler startt,void* arg)
		{
			start=startt;
			handler h=&hand;

			if(pthread_create(thd,NULL,hand,arg)!=0)
			{
				throw std::bad_function_call();
			}
		}

		~thread()
		{
		}

		void join()
		{
			if(pthread_join(*thd,&ret)!=0)
			{
				throw std::bad_function_call();
			}
		}

		void cancel()
		{

		}

		void detach()
		{
			pthread_detach(*thd);
		}
};

}

#endif