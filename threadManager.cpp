#include "threadManager.hpp"

ThreadManager::ThreadManager(int _maxThreads)
{
    curThreads = 1;
    maxThreads = _maxThreads;
    threads = new pthread_t *[maxThreads];
    pthread_mutex_init(lock, NULL);
}

ThreadManager::~ThreadManager()
{
    pthread_mutex_lock(lock);
    for(int i = 0; i < maxThreads; i++ )
    {
	assert( pthread_join(*(threads[i]), NULL) );
    }
    delete[] threads;
    pthread_mutex_unlock(lock);
}

int ThreadManager::addThreads( void *(threadFunc)(void *), void *threadArgs )
{
    pthread_mutex_lock(lock);
    if( curThreads < maxThreads )
    {
	curThreads++;
	threads[curThreads-1] = new pthread_t;
	int retVal = pthread_create( threads[curThreads-1], NULL, threadFunc, threadArgs );
	pthread_mutex_unlock(lock);
	return retVal;
    }
    else
    {
	pthread_mutex_unlock(lock);
	return -1;
    }
}

bool ThreadManager::needMoreThreads()
{
    return (curThreads < maxThreads);
}
