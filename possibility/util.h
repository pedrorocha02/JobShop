#ifndef UTIL_FILE
#define UTIL_FILE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct
{
    int usage;
    int job;

} ActiveMachine;

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
int number_register;
int total;
int total_jobs;
int numMachines, numJobs;
pthread_mutex_t mutex;
int *timerMachineArray;
ActiveMachine *ActiveMachineArray;
struct Graph *graph;
ActiveMachine_register *ActiveMachine_registerArray;

// if 0 it mean that machine is free if it is 1 means that is block
int checkMachineUsage(int machineNumber)
{
    if (ActiveMachineArray[machineNumber].job == -1)
        return ActiveMachineArray[machineNumber].usage;
    return 1;
}
int blockMachineUsage(int machineNumber, int job)
{
    pthread_mutex_lock(&mutex);
    ActiveMachineArray[machineNumber].usage = 1;
    ActiveMachineArray[machineNumber].job = job;
    pthread_mutex_unlock(&mutex);
}
int freeMachineUsage(int machineNumber)
{
    pthread_mutex_lock(&mutex);
    ActiveMachineArray[machineNumber].usage = 0;
    ActiveMachineArray[machineNumber].job = -1;
    pthread_mutex_unlock(&mutex);
}
int updateMachineUsage(int machineNumber, int update)
{
    timerMachineArray[machineNumber] = timerMachineArray[machineNumber] + update;
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
    printf("timer \n");
    for (int i = 0; i < numMachines * numJobs; i++)
    {
        printf("Number %d with Machine %d Job %d started %d end %d \n", i, ActiveMachine_registerArray[i].machine, ActiveMachine_registerArray[i].job, ActiveMachine_registerArray[i].start, ActiveMachine_registerArray[i].end);
    }
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