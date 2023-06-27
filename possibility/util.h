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

int number_register;
int numMachines, numJobs;
struct Graph *graph, *sumgraph;

ActiveMachine_register *ActiveMachine_registerArray;
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
}

///////////////////////
typedef struct
{
    // processing time
    int processing_time;
    // real time
    int real_time;
    // the due date
    int due_date;
} shiftingBottleneckHeuristic;

typedef struct
{
    int job_id;
    // processing time
    int processing_time;
    // real time
    int t;
    // the due date
    int due_date;
} Branch_Bound;
typedef struct
{
    Branch_Bound *EDD_table;
    int machine_number;
} machine_Branch_Bound;

typedef struct
{
    // lowerbound
    int lowerbound;
    // machine_number
    int machine_number;
} machine_settings;

// struct used to use threads in startShiftingBottleneckHeuristictGraph
typedef struct
{
    shiftingBottleneckHeuristic *shiftingbottleneck_table;
    machine_settings machine;
} Info_machine;

typedef struct
{
    int job_id;
    int machine_id;
} Info_thread;

// shiftingBottleneckHeuristic *shiftingbottleneck_table;
Branch_Bound *EDD_table;
Info_machine *info_machine_global;
machine_settings *machine_set;
machine_Branch_Bound *EDD_table_global;

int sortMachinesMaxTimer()
{
    int max = 0, temp_max = 0;
    machine_settings temp;
    // printf("Machine timer max \n");
    for (int i = 0; i < numMachines; i++)
    {
        for (int y = 0; y < numMachines; y++)
        {
            if (machine_set[i].lowerbound > machine_set[y].lowerbound)
            {
                temp = machine_set[i];
                machine_set[i] = machine_set[y];
                machine_set[y] = temp;
            }
        }
    }
    // lowerbound max the minimal value to work
    for (int i = 0; i < numMachines; i++)
    {
        info_machine_global[i].machine.lowerbound = machine_set[i].lowerbound;
        info_machine_global[i].machine.machine_number = machine_set[i].machine_number;
    }
}
int printMachinesMaxTimer()
{
    machine_settings temp;
    // printf("Machine timer max \n");
    for (int i = 0; i < numMachines; i++)
    {
        printf(" value for machine %d with lowerbown = %d \n", machine_set[i].machine_number, machine_set[i].lowerbound);
    }
}

int printMachinesMaxshiftingBottleneckHeuristicTimer()
{
    for (int y = 0; y < numMachines; y++)
    {
        printf("Machine number %d with max %d\n", info_machine_global[y].machine.machine_number, info_machine_global[y].machine.lowerbound);

        for (int i = 0; i < numJobs; i++)
        {
            printf(" value for job %d with p = %d  r = %d dd = %d\n", i, info_machine_global[y].shiftingbottleneck_table[i].processing_time,
                   info_machine_global[y].shiftingbottleneck_table[i].real_time,
                   info_machine_global[y].shiftingbottleneck_table[i].due_date);
        }
    }
}

int printMachineEDDTimer()
{
    int max;
    for (int i = 0; i < numJobs; i++)
    {
        if (max < EDD_table[i].t)
            max = EDD_table[i].t;
        printf(" value for job %d with processing time = %d  due date = %d t = %d\n", EDD_table[i].job_id, EDD_table[i].processing_time, EDD_table[i].due_date, EDD_table[i].t);
    }
    printf("Max value %d\n", max);
}

/////////////////////////////////////////////

#endif