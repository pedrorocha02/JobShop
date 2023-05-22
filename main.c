#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int machineNumber;
    int operationTime;
} Item;

void main()
{
    FILE *file = fopen("input.txt", "r");

    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    FILE *outputFile;
    outputFile = fopen("output.txt", "w");

    if (outputFile == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    int numMachines, numJobs;
    fscanf(file, "%d %d", &numMachines, &numJobs);

    Item items[numMachines * numJobs];

    for (int i = 0; i < numMachines * numJobs; i++)
    {
        fscanf(file, "%d", &items[i].machineNumber);
        fscanf(file, "%d", &items[i].operationTime);
    }

    fclose(file);

    // Printing the read data
    printf("Number of Machines: %d\n", numMachines);
    printf("Number of Jobs: %d\n", numJobs);

    int totalOperationTime = 0;
    int startTime = 0;

    for (int i = 0; i < numMachines * numJobs; i++)
    {
        fprintf(outputFile, "Job Number: %d, Operation Number: %d, Start Time: %d, Machine Number: %d, Operation Time: %d\n",
                i / numMachines, i % numMachines, totalOperationTime, items[i].machineNumber, items[i].operationTime);
        // Worst case scenario
        printf("Machine Number: %d, Operation Time: %d\n", items[i].machineNumber, items[i].operationTime);
        // Sum of the operation total (Similar to a fspan)
        totalOperationTime += items[i].operationTime;
    }

    fclose(outputFile);
    printf("Total Operation Time (including delays): %d\n", totalOperationTime);

    return;
}
