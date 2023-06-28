#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>

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
    pthread_mutex_t barrier_mutex;
    pthread_cond_t barrier_cond;
} Machine;

Machine *machines;
Job *jobs;

// Number of executed operations
int count = 0;
// Number of operations in the system
int totalOperations;

// Used to insure that the program executes all the operations before ending
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *distribute_job(void *arg)
{
    // Parsing the arg to a job

    int jobIndex = *((int *)arg);
    Job *job = &jobs[jobIndex];
    int jobNum = job->id;

    FILE *outputFile = fopen("output.txt", "a+");
    if (outputFile == NULL)
    {
        printf("Error opening the file.\n");
        pthread_exit(NULL);
    }

    for (int i = 0; i < job->size; i++)
    {
        int machineNumber = job->sequence[i].machineNumber;
        int operationTime = job->sequence[i].operationTime;

        Machine *machine = &machines[machineNumber];
        int startTime = 0;

        if (machine != NULL)
        {
            pthread_mutex_lock(&machine->barrier_mutex);

            while (machine->hasJob == 1)
            {
                pthread_cond_wait(&machine->barrier_cond, &machine->barrier_mutex);
            }

            machine->hasJob = 1;

            // Define the startTimes of each operation

            if (i == 0 && machine->totalTime == 0)
            {
                startTime = 0;
            }
            else if (i == 0 && machine->totalTime != 0)
            {
                startTime = machine->totalTime;
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

            pthread_mutex_unlock(&machine->barrier_mutex);

            // Lock to count of the number of executed operations with a protection beacuse it is a global count
            // Lock to allow the write of the outputfile
            pthread_mutex_lock(&count_mutex);

            fprintf(outputFile, "%d,%d,%d,%d,%d\n",
                    jobNum, i, machineNumber, operationTime, startTime);

            fflush(outputFile);

            // Count of the number of executed operations with a protection beacuse it is a global count
            // When the number of executed operations equals the operations in system the signal is transmited

            count++;
            if (count == totalOperations)
            {
                pthread_cond_signal(&count_threshold_cv);
            }
            pthread_mutex_unlock(&count_mutex);

            // Set the hasJob property to 0 and signal that the operation has ended
            pthread_mutex_lock(&machine->barrier_mutex);

            machine->hasJob = 0;
            pthread_cond_signal(&machine->barrier_cond);

            pthread_mutex_unlock(&machine->barrier_mutex);
        }
    }

    fclose(outputFile);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Usage: %s <input_file> <output_file> <num_threads>\n", argv[0]);
        return 1;
    }

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
    int numThreads;
    sscanf(argv[3], "%d", &numThreads);

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
        jobs[i].sequence = malloc(numMachines * sizeof(Item));
        jobs[i].size = numMachines;
        jobs[i].totalTime = 0;

        for (int j = 0; j < numMachines; j++)
        {
            jobs[i].sequence[j] = items[(i * numMachines) + j];
        }
    }

    // Initialize the machines
    machines = (Machine *)malloc(numMachines * sizeof(Machine));

    for (int i = 0; i < numMachines; i++)
    {
        machines[i].id = i;
        machines[i].hasJob = 0;
        machines[i].totalTime = 0;
        machines[i].barrier_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
        machines[i].barrier_cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    }

    // Number of operations in the system
    totalOperations = numJobs * numMachines; // Number of operations

    pthread_t threads[numThreads];
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_threshold_cv, NULL);

    int jobIndices[numJobs];
    for (int i = 0; i < numJobs; i++)
    {
        jobIndices[i] = i;
        pthread_create(&threads[i], NULL, distribute_job, &jobIndices[i]);
    }

    pthread_mutex_lock(&count_mutex);

    // Wait until the number of executed operations is equal to the numeber of operations in the system
    while (count < totalOperations)
    {
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
    }
    pthread_mutex_unlock(&count_mutex);

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Free the variables
    free(items);
    for (int i = 0; i < numJobs; i++)
    {
        free(jobs[i].sequence);
    }
    free(jobs);

    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);

    return 0;
}
