#ifndef SEQUENCIAL_FILE
#define SEQUENCIAL_FILE

void printSequencialGraph()
{
    int v;
    total = 0;
    total_jobs = 0;
    number_register = 0;
    int level = 0;
    int count = 0;
    for (int i = 1; i <= numJobs; i++)
    {
        printf("jobs_id-> %d \n", i);
        total = 0;
        level = 1;
        for (v = 0; v < graph->V; ++v)
        {
            struct AdjListNode *pCrawl = graph->array[v].head;
            while (pCrawl)
            {
                if (pCrawl->jobs_id == i && pCrawl->dest == level)
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
        total_jobs = total_jobs + total;
    }
    printf("total of the total -> %d \n", total_jobs);
}

#endif