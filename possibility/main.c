#include "util.h"
#include "graph.h"
#include "threads.h"

int main(int argc, char **argv)
{
    int startTime = 0;

    int i;
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

    ActiveMachine_registerArray = malloc(numMachines * numJobs * sizeof(ActiveMachine_register));

    ////
    info_machine_global = malloc(numMachines * sizeof(Branch_Bound));

    EDD_table_global = malloc(numMachines * sizeof(machine_Branch_Bound));

    for (int i = 0; i < numMachines; i++)
    {
        info_machine_global[i].shiftingbottleneck_table = malloc(numJobs * sizeof(shiftingBottleneckHeuristic));

        EDD_table_global[i].EDD_table = malloc(numJobs * sizeof(Branch_Bound));
    }

    EDD_table = malloc(numJobs * sizeof(Branch_Bound));
    machine_set = malloc(numMachines * sizeof(machine_settings));

    /////

    graph = createGraph(numJobs);
    // begingraph = graph;
    sumgraph = createGraph(numJobs);

    int level = 0;
    int job_number = -1;

    for (int i = 0; i < numMachines * numJobs; i++, level++)
    {

        fscanf(file, "%d", &items[i].machineNumber);
        fscanf(file, "%d", &items[i].operationTime);
        // to make sure that eversumgraphy node starts in the same stage
        if (level % numMachines == 0)
        {
            level = 0;
            job_number++;
        }
        addEdge(sumgraph, job_number, job_number + 1, items[i].operationTime, level, items[i].machineNumber);
        addEdge(graph, job_number, job_number + 1, items[i].operationTime, level, items[i].machineNumber);
        machine_set[items[i].machineNumber].lowerbound = machine_set[items[i].machineNumber].lowerbound + items[i].operationTime;
        machine_set[items[i].machineNumber].machine_number = items[i].machineNumber;
    }

    fclose(file);

    /* organize the matrix of the max maxines*/
    sortMachinesMaxTimer();
    printMachinesMaxTimer();
    // Printing the read data
    printf("Number of Machines: %d\n", numMachines);
    printf("Number of Jobs: %d\n", numJobs);
    // printGraph(graph);
    startsumgraph();
    // printGraph(sumgraph);
    printf("-----------------------------------------------------------------\n");
    startShiftingBottleneckHeuristictGraph();
    printMachinesMaxshiftingBottleneckHeuristicTimer();
    updateGraph();
    // printGraph(sumgraph);
    ///////////////////////////////////////////////////////
    /*
        printf("\nBest sequencial\n");
        begin = clock();
        startBestGraphthread();
        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Time runnning this process %lf", time_spent);

        //    printMachinesTimer();
        //    printJobsTimer();
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
        */
    /*
      clearMachinesTimer();
      clearJobsTimer();
      clearMachinesRegisterTimer();
  */
    deleteGraph(graph);

    return 0;
}
