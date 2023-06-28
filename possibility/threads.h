#ifndef THREADS_FILE
#define THREADS_FILE

void *ShiftingBottleneckHeuristic(void *vargp)
{
    int passed = 0;
    Info_thread *recived = (Info_thread *)vargp;
    int job_id = recived->job_id;
    int machine_id = recived->machine_id;
    int level = 0;

    info_machine_global[machine_id].shiftingbottleneck_table[job_id].real_time = 0;

    struct AdjListNode *pCrawl = graph->array[job_id].head;
    while (pCrawl)
    {
        if (passed == 1)
        {
            // this for remove the final process of the lowerbound
            info_machine_global[machine_id].shiftingbottleneck_table[job_id].due_date = info_machine_global[machine_id].shiftingbottleneck_table[job_id].due_date - pCrawl->duration;
        }
        if (info_machine_global[machine_id].machine.machine_number == pCrawl->machine)
        {
            // in the machine the processing time will be the duration of the machine
            info_machine_global[machine_id].shiftingbottleneck_table[job_id].processing_time = pCrawl->duration;
            passed = 1;
            // add the lowerbound the is the max process of this machine
            info_machine_global[machine_id].shiftingbottleneck_table[job_id].due_date = info_machine_global[machine_id].machine.lowerbound;
        }
        if (passed == 0)
        {
            // add the total time of this job for machine info_machine_global[machine_id].machine.machine_number
            info_machine_global[machine_id].shiftingbottleneck_table[job_id].real_time = info_machine_global[machine_id].shiftingbottleneck_table[job_id].real_time + pCrawl->duration;
        }
        pCrawl = pCrawl->next;
    }
}

void *run_sumgraph(void *vargp)
{
    int passed = 0;
    Info_thread *job_id_recived = (long *)vargp;
    int job_id = (int)job_id_recived;
    int level = 0;
    int val = 0;

    struct AdjListNode *pCrawl = sumgraph->array[job_id].head;
    while (pCrawl)
    {
        pCrawl->duration = pCrawl->duration + val;
        val = pCrawl->duration;

        pCrawl = pCrawl->next;
    }
}

void runMachineEDDTimer(int machine_number)
{
    int passed = 0;
    int v, i, flag = 0;
    int max;
    int passetvalue = 0;
    // is allaways considering j1,j2,j3 the best combination we were not abled to do the combinations between all jobs
    for (v = 0; v < numJobs; v++)
    {
        // tempo maximo de processamento
        EDD_table_global[machine_number].EDD_table[v].due_date = info_machine_global[machine_number].shiftingbottleneck_table[v].due_date;
        EDD_table_global[machine_number].machine_number = machine_number;
        EDD_table_global[machine_number].EDD_table[v].job_id = v;
        // passetvelue that start with zero and is from the last interation
        //  if the passetvalue from tha last interation is smaller them the real_time of the machine
        //  it will update processing_time with processing_time shiftingbottleneck process and real machine
        // in other case it will update with value of the value of the last iteration
        if (passetvalue < info_machine_global[machine_number].shiftingbottleneck_table[v].real_time)
        {
            EDD_table_global[machine_number].EDD_table[v].processing_time = info_machine_global[machine_number].shiftingbottleneck_table[v].processing_time + info_machine_global[machine_number].shiftingbottleneck_table[v].real_time;
        }
        else
        {
            EDD_table_global[machine_number].EDD_table[v].processing_time = info_machine_global[machine_number].shiftingbottleneck_table[v].processing_time + passetvalue;
        }
        // the due date is small than processing time remove
        // from the processint time the due date
        if (EDD_table_global[machine_number].EDD_table[v].due_date < EDD_table_global[machine_number].EDD_table[v].processing_time)
        {
            EDD_table_global[machine_number].EDD_table[v].t = EDD_table_global[machine_number].EDD_table[v].processing_time - EDD_table_global[machine_number].EDD_table[v].due_date;
        }
        passetvalue = EDD_table_global[machine_number].EDD_table[v].processing_time;
    }
}

void startShiftingBottleneckHeuristictGraph()
{

    int ret = 0;
    int decrease_thread = 0;
    long thread;
    pthread_t *thread_handles;
    Info_thread *send;
    send = malloc(numJobs * numMachines * sizeof(Info_thread));
    thread_handles = malloc(numJobs * numMachines * sizeof(pthread_t));

    for (int i = 0; i < numMachines; i++)
    {
        for (thread = 0; thread < numJobs;)
        {
            send = malloc(numJobs * numMachines * sizeof(Info_thread));
            send->job_id = thread;
            send->machine_id = i;
            pthread_create(&thread_handles[thread], NULL, ShiftingBottleneckHeuristic, (Info_thread *)send);
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
        runMachineEDDTimer(i);
    }
    free(thread_handles);
}
void startsumgraph()
{

    int ret = 0;
    int decrease_thread = 0;
    long thread;
    pthread_t *thread_handles;
    thread_handles = malloc(numJobs * sizeof(pthread_t));

    for (thread = 0; thread < numJobs;)
    {
        pthread_create(&thread_handles[thread], NULL, run_sumgraph, (void *)thread);
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
}

void updateGraph()
{

    int v;
    int new_val = 0;
    int flag = 0;
    for (v = 0; v < numMachines; ++v)
    {
        new_val = 0;
        flag = 0;
        for (int i = 0; i < numJobs; ++i)
        {

            struct AdjListNode *pCrawl = sumgraph->array[EDD_table_global[info_machine_global[v].machine.machine_number].EDD_table[i].job_id].head;

            while (pCrawl)
            {
                if (pCrawl->machine == info_machine_global[v].machine.machine_number)
                {
                    pCrawl->duration = pCrawl->duration + new_val;
                    flag = 1;
                }
                if (flag == 0)
                    new_val = new_val + pCrawl->duration;
                pCrawl = pCrawl->next;
            }
        }
    }
}

#endif