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

// global variables
int total_jobs_timer;
int number_register;
int numMachines, numJobs;
pthread_mutex_t mutex;
int *timerMachineArray;
struct Graph *graph;
ActiveMachine_register *ActiveMachine_registerArray;

int updateMachineUsage(int machineNumber, int update)
{
    int time_now;
    pthread_mutex_lock(&mutex);
    time_now = timerMachineArray[machineNumber];
    timerMachineArray[machineNumber] = timerMachineArray[machineNumber] + update;
    total_jobs_timer = total_jobs_timer + update;
    pthread_mutex_unlock(&mutex);
    return time_now;
}
int printMachinesTimer()
{
    printf("timer \n");
    for (int i = 0; i < numMachines; i++)
    {
        printf("Machine %d with %d \n", i, timerMachineArray[i]);
    }
}
int clearMachinesTimer()
{
    for (int i = 0; i < numMachines; i++)
    {
        timerMachineArray[i] = 0;
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
    printf("Total of time %d\n", total_time);
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