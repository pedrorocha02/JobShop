#ifndef SEQUENCIAL_FILE
#define SEQUENCIAL_FILE

void printSequencialGraph()
{
    total_jobs_timer = 0;
    int time_now = 0;
    int v;
    number_register = 0;
    int level = 0;
    int count = 0;
    initMutexMachines();

    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode *pCrawl = graph->array[v].head;
        while (pCrawl)
        {

            updateRegisterUsage(pCrawl->machine, pCrawl->jobs_id, time_now, time_now + pCrawl->duration);
            time_now = time_now + pCrawl->duration;
            pCrawl = pCrawl->next;
        }
    }
}

#endif