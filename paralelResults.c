#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);
    clock_t start_time, end_time;
    double execution_time;

    start_time = clock();

    // Compile and execute sequential.c
    system("gcc -pthread main.c -o main");
    for (int i = 0; i < 100; i++)
    {
        system("D:/CAD/TrabalhoGrupo/main.exe input.txt output.txt %d", numThreads);
    }

    end_time = clock();
    execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Open the results.txt file for writing
    FILE *file = fopen("results.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    // Write the execution time to the file
    fprintf(file, "Execution time: %lf seconds\n", execution_time);

    fclose(file);

    return 0;
}
