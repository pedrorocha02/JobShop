#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int machineNumber;
    int operationTime;
} Item;

typedef struct
{
    int id;
    Item *sequence;
    int size;
    int totalTime;
} Job;

typedef struct
{
    int id;
    int hasJob;
    int totalTime;
} Machine;

Machine *machines;
Job *jobs;

// Number of executed operations
int count = 0;
// Number of operations in the system
int totalOperations;

void distribute_job(int jobIndex)
{
    Job *job = &jobs[jobIndex];
    int jobNum = job->id;

    FILE *outputFile = fopen("output.txt", "a+");
    if (outputFile == NULL)
    {
        printf("Error opening the file.\n");
        exit(1);
    }

    for (int i = 0; i < job->size; i++)
    {

        int machineNumber = job->sequence[i].machineNumber;
        int operationTime = job->sequence[i].operationTime;

        Machine *machine = &machines[machineNumber];
        int startTime = 0;

        if (machine != NULL)
        {
            machine->hasJob = 1;

            // Define the startTimes of each operation
            if (i == 0 && machine->totalTime == 0)
            {
                startTime = 0;
            }
            else if (i == 0 && machine->totalTime != 0)
            {
                startTime = machine->totalTime;
                job->totalTime = startTime;
            }
            else if (i != 0 && machine->totalTime == 0)
            {
                startTime = job->totalTime;
                machine->totalTime = startTime;
            }
            else if (i != 0 && machine->totalTime != 0)
            {
                if (machine->totalTime > job->totalTime)
                {
                    startTime = machine->totalTime;
                    job->totalTime = startTime;
                }
                else
                {
                    startTime = job->totalTime;
                    machine->totalTime = startTime;
                }
            }

            machine->totalTime += operationTime;
            job->totalTime += operationTime;

            // Write to the output file
            fprintf(outputFile, "%d,%d,%d,%d,%d\n",
                    jobNum, i, machineNumber, operationTime, startTime);

            fflush(outputFile);

            count++;
            machine->hasJob = 0;
        }
    }

    fclose(outputFile);
}

int main(int argc, char **argv)
{
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Failed to open the input file.\n");
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "w");
    if (outputFile == NULL)
    {
        printf("Failed to open the output file.\n");
        fclose(file);
        return 1;
    }

    // Output file header
    const char *outputFileHeader = "Job Number,Operation Number,Machine Number,Operation Time,Start Time\n";
    fprintf(outputFile, "%s", outputFileHeader);

    fclose(outputFile);

    outputFile = fopen(argv[2], "a+");
    if (outputFile == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    // Get the num of threads
    int numMachines, numJobs;
    fscanf(file, "%d %d", &numMachines, &numJobs);

    Item *items = malloc(numMachines * numJobs * sizeof(Item));

    for (int i = 0; i < numMachines * numJobs; i++)
    {
        fscanf(file, "%d", &items[i].machineNumber);
        fscanf(file, "%d", &items[i].operationTime);
    }

    fclose(file);

    // Initialize the jobs
    jobs = (Job *)malloc(numJobs * sizeof(Job));

    for (int i = 0; i < numJobs; i++)
    {
        jobs[i].id = i;
        jobs[i].sequence = malloc(numJobs * sizeof(Item));
        jobs[i].size = numJobs;
        jobs[i].totalTime = 0;

        for (int j = 0; j < numJobs; j++)
        {
            jobs[i].sequence[j] = items[(i * numJobs) + j];
        }
    }

    // Initialize the machines
    machines = (Machine *)malloc(numMachines * sizeof(Machine));

    for (int i = 0; i < numMachines; i++)
    {
        machines[i].id = i;
        machines[i].hasJob = 0;
        machines[i].totalTime = 0;
    }

    // Number of operations in the system
    totalOperations = numJobs * numMachines; // Number of operations
    // printf("%d", totalOperations);

    for (int i = 0; i < numJobs; i++)
    {
        distribute_job(i);
    }

    // Free the variables
    free(items);
    for (int i = 0; i < numJobs; i++)
    {
        free(jobs[i].sequence);
    }
    free(jobs);

    return 0;
}
