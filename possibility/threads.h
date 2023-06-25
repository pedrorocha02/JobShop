#ifndef THREADS_FILE
#define THREADS_FILE

void *Increment(void *vargp)
{
    int time_now;
    int *id_job = (long *)vargp;
    int v = id_job;
    int level = 0;
    printf("jobs_id-> %d \n", id_job);

    struct AdjListNode *pCrawl = graph->array[v].head;
    while (pCrawl)
    {

        printf("jobs_id-> %d level -> %d machine -> %d duration -> %d \n", pCrawl->jobs_id, pCrawl->level, pCrawl->machine, pCrawl->duration);
        updateMachineUsage(pCrawl->machine, pCrawl->duration, pCrawl->jobs_id, level);
        pCrawl = pCrawl->next;
        level++;
    }
}

void startBestGraphthread(int NumThreads)
{
    if (NumThreads == 0)
    {
        NumThreads = numJobs;
    }
    total_jobs_timer = 0;
    number_register = 0;
    int ret = 0;
    int decrease_thread = 0;
    long thread;
    pthread_t *thread_handles;
    thread_handles = malloc(numJobs * sizeof(pthread_t));

    initMutexMachines();

    for (thread = 0; thread < numJobs;)
    {
        ret = pthread_create(&thread_handles[thread], NULL, Increment, (void *)thread);
        if (ret == 0)
        {
            thread++;
        }
    }

    ret = 0;
    for (thread = 0; thread < numJobs;)
    {
        ret = pthread_join(thread_handles[thread], NULL);
        if (ret == 0)
        {
            thread++;
        }
    }

    free(thread_handles);
    // printf("total of the total -> %d \n", total_jobs);
}

#endif