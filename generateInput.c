#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_JOBS 500
#define NUM_MACHINES 500

void shuffle(int array[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int j = i + rand() / (RAND_MAX / (size - i) + 1);
        int temp = array[j];
        array[j] = array[i];
        array[i] = temp;
    }
}

int main()
{
    int jobs[NUM_JOBS][NUM_MACHINES][2];
    int machine_order[NUM_MACHINES];
    srand(time(NULL)); // Seed the random number generator

    // Generate the initial machine order
    for (int i = 0; i < NUM_MACHINES; i++)
    {
        machine_order[i] = i;
    }

    // Generate the machine numbers and operation times for each job
    for (int i = 0; i < NUM_JOBS; i++)
    {
        shuffle(machine_order, NUM_MACHINES); // Shuffle the machine order
        for (int j = 0; j < NUM_MACHINES; j++)
        {
            jobs[i][j][0] = machine_order[j];
            jobs[i][j][1] = rand() % 30 + 1; // Random operation time between 1 and 4
        }
    }

    // Write the instance to input.txt
    FILE *file = fopen("input.txt", "w");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1;
    }

    fprintf(file, "%d %d\n", NUM_JOBS, NUM_MACHINES);
    for (int i = 0; i < NUM_JOBS; i++)
    {
        for (int j = 0; j < NUM_MACHINES; j++)
        {
            fprintf(file, "%d %d ", jobs[i][j][0], jobs[i][j][1]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}
