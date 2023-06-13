#ifndef THREADS_FILE
#define THREADS_FILE

void *Increment(void *vargp)
{

    int *id_job = (long *)vargp;
    int v;
    int level = 0;
    printf("jobs_id-> %d \n", id_job);
    total = 0;
    level = 1;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode *pCrawl = graph->array[v].head;
        while (pCrawl)
        {
            if (pCrawl->jobs_id == id_job && pCrawl->dest == level)
            {
                printf("jobs_id-> %d level -> %d machine -> %d duration -> %d \n", pCrawl->jobs_id, pCrawl->dest, pCrawl->machine, pCrawl->duration);
                blockMachineUsage(pCrawl->machine, pCrawl->jobs_id);
                updateMachineUsage(pCrawl->machine, pCrawl->duration);
                updateRegisterUsage(pCrawl->machine, pCrawl->jobs_id, total, total + pCrawl->duration);
                total = total + pCrawl->duration;
                total_jobs = total_jobs + pCrawl->duration;
                freeMachineUsage(pCrawl->machine);
                level++;
            }
            pCrawl = pCrawl->next;
        }
    }
    // printf("total  -> %d \n", total);
    // total_jobs = total_jobs + total;
    // printf("total of the total -> %d \n", total_jobs);
}

void printBestGraphthread()
{
    number_register = 0;
    total = 0;
    total_jobs = 0;
    long thread;
    pthread_t *thread_handles;
    thread_handles = malloc(numJobs * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);

    for (thread = 1; thread <= numJobs; thread++)
        pthread_create(&thread_handles[thread], NULL, Increment, (void *)thread);

    for (thread = 1; thread <= numJobs; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
    printf("total of the total -> %d \n", total_jobs);
}

#endif