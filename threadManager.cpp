#include "threadManager.hpp"

ThreadManager::ThreadManager(int _maxThreads)
{
    curThreads = 1;
    maxThreads = _maxThreads;
    threads = new pthread_t *[maxThreads];
    master = pthread_self();
    privateLock = new pthread_mutex_t;
    publicLock = new pthread_mutex_t;
    pthread_mutex_init(privateLock, NULL);
    pthread_mutex_init(publicLock, NULL);
}

ThreadManager::~ThreadManager()
{
    pthread_mutex_lock(privateLock);
    for(int i = 0; i < maxThreads; i++ )
    {
	if( threads[i] != NULL )
	{
	    ( pthread_join(*(threads[i]), NULL) );
	}
    }
    delete[] threads;
    pthread_mutex_unlock(privateLock);
    delete privateLock;
    delete publicLock;
}

int ThreadManager::barrier()
{
    while(true)
    {
	pthread_mutex_lock(privateLock);
	std::cout << "!!! Waiting at barrier, number of running threads = " << curThreads << std::endl;
	if( curThreads <= 1 )
	{
	    break;
	}
	pthread_mutex_unlock(privateLock);
	usleep(10.0e3);
    }

    std::cout << "*** Waiting at barrier until all threads finish ***" << std::endl;
    for(int i = 0; i < maxThreads; i++ )
    {	
	if( threads[i] != NULL )
	{
	    std::cout << "*** Waiting for thread " << i << ", " << *(threads[i]) << "***" << std::endl;
	    ( pthread_join(*(threads[i]), NULL) );
	}
    }
    pthread_mutex_unlock(privateLock);
    return 0;
}

int ThreadManager::masterBarrier()
{
    if( pthread_equal( master, pthread_self() ))
    {
	while(true)
	{	
	    pthread_mutex_lock(privateLock);
	    std::cout << "!!! Master waiting at barrier, number of running threads = " << curThreads << std::endl;
	    if( curThreads <= 1 )
	    {
		break;
	    }
	    pthread_mutex_unlock(privateLock);
	    usleep(10.0e3);
	}
	pthread_mutex_unlock(privateLock);
	return 0;
    }
    else
    {
	return 0;
    }
}

int ThreadManager::addThreads( void *(threadFunc)(void *), void *threadArgs )
{
    int retVal = pthread_mutex_trylock(privateLock);
    if( retVal == 0 && pthread_equal( master, pthread_self() ))
    {
	std::cout << "*** Thread " << pthread_self() << " is launching a new thread ***" << std::endl;
	if( curThreads < maxThreads )
	{
	    curThreads++;
	    threads[curThreads-1] = new pthread_t;
	    retVal = pthread_create( threads[curThreads-1], NULL, threadFunc, threadArgs );
	    pthread_mutex_unlock(privateLock);
	    return retVal;
	}
	else
	{
	    pthread_mutex_unlock(privateLock);
	    return -1;
	}
    }
    else
    {
	pthread_mutex_unlock(privateLock);
	return -1;
    }
}

void ThreadManager::removeThreads()
{
    pthread_mutex_lock(privateLock);

    if( curThreads > 1 )
	curThreads--;
    pthread_mutex_unlock(privateLock);
}

bool ThreadManager::needMoreThreads()
{
    return (curThreads < maxThreads);
}
