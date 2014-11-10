class threadManager
{
private:
    int		    maxThreads;
    int		    curThreads;
    pthread_mutex_t lock;
    pthread_t	    *threads;
public:
    threadManager(int _maxThreads);
    ~threadManager();
    int addThreads( void *(threadFunc)(void *), void *threadArgs );
    bool needMoreThreads();
}
