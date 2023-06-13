#include "util.h"
#include "graph.h"
#include "sequencial.h"
#include "threads.h"

int main(int argc, char **argv)
{
    int totalOperationTime = 0;
    int startTime = 0;

    int i;

    FILE *file = fopen("input.txt", "r");

    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 0;
    }

    FILE *outputFile;
    outputFile = fopen("output.txt", "w");

    if (outputFile == NULL)
    {
        printf("Error opening the file.\n");
        return 0;
    }

    fscanf(file, "%d %d", &numMachines, &numJobs);

    Item items[numMachines * numJobs];

    timerMachineArray = malloc(numMachines * sizeof(int));
    ActiveMachineArray = malloc(numMachines * sizeof(ActiveMachine));
    ActiveMachine_registerArray = malloc(numMachines * numJobs * sizeof(ActiveMachine_register));

    graph = createGraph(numMachines);

    int level = 0;
    int job_number = 0;

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
        addEdge(graph, level, level + 1, items[i].operationTime, job_number, items[i].machineNumber);
    }

    fclose(file);

    // Printing the read data
    printf("Number of Machines: %d\n", numMachines);
    printf("Number of Jobs: %d\n", numJobs);
    printGraph(graph);
    printf("Test sequencial\n");
    printSequencialGraph();

    printMachinesTimer();
    printMachinesRegisterTimer();
    clearMachinesTimer();
    clearMachinesRegisterTimer();

    printf("\nBest sequencial\n");
    printBestGraphthread();
    printMachinesTimer();
    printMachinesRegisterTimer();

    clearMachinesTimer();
    clearMachinesRegisterTimer();

    free(timerMachineArray);
    free(ActiveMachineArray);
    deleteGraph(graph);

    return 0;
}
