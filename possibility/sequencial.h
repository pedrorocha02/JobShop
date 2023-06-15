#ifndef SEQUENCIAL_FILE
#define SEQUENCIAL_FILE

void printSequencialGraph()
{
    int time_now = 0;
    int v;
    number_register = 0;
    int level = 0;
    int count = 0;
    for (int i = 1; i <= numJobs; i++)
    {
        // printf("jobs_id-> %d \n", i);
        level = 1;
        for (v = 0; v < graph->V; ++v)
        {
            struct AdjListNode *pCrawl = graph->array[v].head;
            while (pCrawl)
            {
                if (pCrawl->jobs_id == i && pCrawl->dest == level)
                {
                    // printf("jobs_id-> %d level -> %d machine -> %d duration -> %d \n", pCrawl->jobs_id, pCrawl->dest, pCrawl->machine, pCrawl->duration);
                    //  blockMachineUsage(pCrawl->machine, pCrawl->jobs_id);
                    updateMachineUsage(pCrawl->machine, pCrawl->duration);
                    updateRegisterUsage(pCrawl->machine, pCrawl->jobs_id, time_now, time_now + pCrawl->duration);
                    time_now = time_now + pCrawl->duration;
                    // freeMachineUsage(pCrawl->machine);
                    level++;
                }
                pCrawl = pCrawl->next;
            }
        }
    }
}

#endif