#include "util.h"
#include "graph.h"
#include "sequencial.h"
#include "threads.h"

int main(int argc, char **argv)
{
    int totalOperationTime = 0;
    int startTime = 0;

    int i;
    int number_of_threadas;
    clock_t begin,
        end;
    double time_spent;
    FILE *file, *outputFile;

    if (argc < 3)
    {
        printf("This program shoed run like this  ./main input.txt output.txt (number of threads) \n");
        return -1;
    };

    file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 0;
    }

    fscanf(file, "%d %d", &numJobs, &numMachines);

    Item items[numMachines * numJobs];

    timerMachineArray = malloc(numMachines * sizeof(Machine_item));
    timeMachineMax = malloc(numMachines * sizeof(int));
    timerJobsArray = malloc((numJobs + 1) * sizeof(Job_item));
    ActiveMachine_registerArray = malloc(numMachines * numJobs * sizeof(ActiveMachine_register));

    graph = createGraph(numJobs);

    int level = 0;
    int job_number = -1;

    for (int i = 0; i < numMachines * numJobs; i++, level++)
    {

        fscanf(file, "%d", &items[i].machineNumber);
        fscanf(file, "%d", &items[i].operationTime);
        // to make sure that every node starts in the same stage
        if (level % numMachines == 0)
        {
            level = 0;
            job_number++;
        }
        addEdge(graph, job_number, job_number + 1, items[i].operationTime, level, items[i].machineNumber);
        timeMachineMax[items[i].machineNumber] = timeMachineMax[items[i].machineNumber] + items[i].operationTime;
    }

    fclose(file);

    /* here, do your time-consuming job */

    // Printing the read data
    printf("Number of Machines: %d\n", numMachines);
    printf("Number of Jobs: %d\n", numJobs);

    // printGraph(graph);
    // printMachinesMaxTimer();
    /*printf("Test sequencial\n");
    begin = clock();
    printSequencialGraph();
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time runnning this process %lf", time_spent);

    printMachinesTimer();
    printJobsTimer();
    printMachinesRegisterTimer();
    clearMachinesTimer();
    clearJobsTimer();
    clearMachinesRegisterTimer();
*/

    printf("\nBest sequencial\n");
    begin = clock();
    sscanf(argv[3], "%d", &number_of_threadas);
    startBestGraphthread(number_of_threadas);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time runnning this process %lf", time_spent);

    printMachinesTimer();
    printJobsTimer();
    printMachinesRegisterTimer();

    outputFile = fopen(argv[2], "w");

    if (outputFile == NULL)
    {
        printf("Error opening the file.\n");
        return 0;
    }
    fprintf(outputFile, "job id; operation ;start time ; machine id; operation time \n");
    for (int i = 0; i < numMachines * numJobs; i++)
    {
        fprintf(outputFile, "%d ;%d ;%d ; %d; %d \n", ActiveMachine_registerArray[i].job, i, ActiveMachine_registerArray[i].start, ActiveMachine_registerArray[i].machine, ActiveMachine_registerArray[i].end - ActiveMachine_registerArray[i].start);
        // Worst case scenario
        // printf("Machine Number: %d, Operation Time: %d\n", items[i].machineNumber, items[i].operationTime);
    }

    fclose(outputFile);

    clearMachinesTimer();
    clearJobsTimer();
    clearMachinesRegisterTimer();

    free(timerMachineArray);
    deleteGraph(graph);

    return 0;
}
