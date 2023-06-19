#ifndef UTIL_FILE
#define UTIL_FILE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

typedef struct
{
    int job;
    int start;
    int end;
    int machine;

} ActiveMachine_register;

typedef struct
{
    int machineNumber;
    int operationTime;
} Item;

typedef struct
{
    int operationTime;
    pthread_mutex_t mutex;
} Machine_item;

// global variables
int total_jobs_timer;
int number_register;
int numMachines, numJobs;
int *timerJobsArray;
struct Graph *graph;
ActiveMachine_register *ActiveMachine_registerArray;
Machine_item *timerMachineArray;

int updateJobUsage(int JobNumber, int update)
{

    timerJobsArray[JobNumber] = timerJobsArray[JobNumber] + update;
}
int printJobsTimer()
{
    printf("Jobs timer \n");
    for (int i = 1; i <= numJobs; i++)
    {
        printf("Job %d with %d \n", i, timerJobsArray[i]);
    }
}
int clearJobsTimer()
{
    for (int i = 1; i <= numJobs; i++)
    {
        timerJobsArray[i] = 0;
    }
}

int updateMachineUsage(int machineNumber, int update)
{
    int time_now;
    pthread_mutex_lock(&timerMachineArray[machineNumber].mutex);
    time_now = timerMachineArray[machineNumber].operationTime;
    timerMachineArray[machineNumber].operationTime = timerMachineArray[machineNumber].operationTime + update;
    total_jobs_timer = total_jobs_timer + update;
    pthread_mutex_unlock(&timerMachineArray[machineNumber].mutex);
    return time_now;
}
int printMachinesTimer()
{
    printf("Machine timer \n");
    for (int i = 0; i < numMachines; i++)
    {
        printf("Machine %d with %d \n", i, timerMachineArray[i].operationTime);
    }
}
int clearMachinesTimer()
{
    for (int i = 0; i < numMachines; i++)
    {
        timerMachineArray[i].operationTime = 0;
        pthread_mutex_destroy(&timerMachineArray[i].mutex);
    }
}

int initMutexMachines()
{
    for (int i = 0; i < numMachines; i++)
    {
        pthread_mutex_init(&timerMachineArray[i].mutex, NULL);
    }
}

int updateRegisterUsage(int machine, int job, int start, int end)
{

    ActiveMachine_registerArray[number_register].job = job;
    ActiveMachine_registerArray[number_register].machine = machine;
    ActiveMachine_registerArray[number_register].start = start;
    ActiveMachine_registerArray[number_register].end = end;
    number_register++;
}
int printMachinesRegisterTimer()
{
    int total_time = 0;
    printf("timer \n");
    for (int i = 0; i < numMachines * numJobs; i++)
    {
        if (total_time < ActiveMachine_registerArray[i].end)
        {
            total_time = ActiveMachine_registerArray[i].end;
        }
        printf("Number %d with Machine %d Job %d started %d end %d \n", i, ActiveMachine_registerArray[i].machine, ActiveMachine_registerArray[i].job, ActiveMachine_registerArray[i].start, ActiveMachine_registerArray[i].end);
    }
    printf("Total of time of all the machines %d\n", total_jobs_timer);
    printf("Max time of machine  %d\n", total_time);
}
int clearMachinesRegisterTimer()
{
    for (int i = 0; i < numMachines * numJobs; i++)
    {
        ActiveMachine_registerArray[i].machine = -1;
        ActiveMachine_registerArray[i].job = -1;
        ActiveMachine_registerArray[i].start = -1;
        ActiveMachine_registerArray[i].end = -1;
    }
}
#endif