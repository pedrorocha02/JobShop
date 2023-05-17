#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int machineNumber;
    int operationTime;
} Item;

int main()
{
    FILE *file = fopen("input.txt", "r"); // Replace "input.txt" with the actual filename

    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1;
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

    for (int i = 0; i < numMachines * numJobs; i++)
    {
        printf("Machine Number: %d, Operation Time: %d\n", items[i].machineNumber, items[i].operationTime);
    }

    return 0;
}
