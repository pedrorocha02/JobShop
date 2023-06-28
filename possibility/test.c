#include <stdio.h>

#define MAX_JOBS 10
#define MAX_MACHINES 10

// https://github.com/ptal/kobe-scheduling/blob/master/data/jssp/ft/ft06.jss
// https://ptal.github.io/scheduling-data.html#job-shop-scheduling-problem

int time[MAX_JOBS][MAX_MACHINES];
int sequence[MAX_JOBS][MAX_MACHINES];
int schedule[MAX_JOBS][MAX_MACHINES];

void calculateSchedule(int numJobs, int numMachines)
{
    int machineTime[MAX_MACHINES] = {0};

    for (int i = 0; i < numJobs; i++)
    {
        for (int j = 0; j < numMachines; j++)
        {
            int machine = sequence[i][j];
            int jobTime = time[i][machine];
            int startTime = (machineTime[machine] > jobTime) ? machineTime[machine] : jobTime;

            schedule[i][machine] = startTime;
            machineTime[machine] = startTime + jobTime;
        }
    }
}

void printSchedule(int numJobs, int numMachines)
{
    printf("Job Schedule:\n");

    for (int i = 0; i < numJobs; i++)
    {
        printf("Job %d:", i + 1);

        for (int j = 0; j < numMachines; j++)
        {
            printf(" Machine %d (%d)", sequence[i][j] + 1, schedule[i][sequence[i][j]]);
        }

        printf("\n");
    }
}

int main()
{
    int numJobs, numMachines;

    printf("Enter the number of jobs: ");
    scanf("%d", &numJobs);

    printf("Enter the number of machines: ");
    scanf("%d", &numMachines);

    printf("Enter the processing times:\n");

    for (int i = 0; i < numJobs; i++)
    {
        printf("Job %d:\n", i + 1);

        for (int j = 0; j < numMachines; j++)
        {
            printf("Machine %d: ", j + 1);
            scanf("%d", &time[i][j]);
        }
    }

    printf("Enter the job sequences:\n");

    for (int i = 0; i < numJobs; i++)
    {
        printf("Job %d: ", i + 1);

        for (int j = 0; j < numMachines; j++)
        {
            scanf("%d", &sequence[i][j]);
            sequence[i][j]--;
        }
    }

    calculateSchedule(numJobs, numMachines);
    printSchedule(numJobs, numMachines);

    return 0;
}
