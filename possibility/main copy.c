#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <omp.h>
#include <stdbool.h>

// from https://www.geeksforgeeks.org/graph-and-its-representations/
// A structure to represent an adjacency list node
struct AdjListNode
{
    int dest;
    struct AdjListNode *next;
};

struct AdjList
{
    struct AdjListNode *head;
};

// A structure to represent a graph. A graph
// is an array of adjacency lists.
// Size of array will be V (number of vertices
// in graph)
struct Graph
{
    int V;
    struct AdjList *array;
};

// A utility function to create a new adjacency list node
struct AdjListNode *newAdjListNode(int dest)
{
    struct AdjListNode *newNode = (struct AdjListNode *)malloc(
        sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of
    // array will be V
    graph->array = (struct AdjList *)malloc(
        V * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by
    // making head as NULL
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph *graph, int src, int dest)
{
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the beginning
    struct AdjListNode *check = NULL;
    struct AdjListNode *newNode = newAdjListNode(dest);

    if (graph->array[src].head == NULL)
    {
        newNode->next = graph->array[src].head;
        graph->array[src].head = newNode;
    }
    else
    {

        check = graph->array[src].head;
        while (check->next != NULL)
        {
            check = check->next;
        }
        // graph->array[src].head = newNode;
        check->next = newNode;
    }

    // Since graph is undirected, add an edge from
    // dest to src also
    newNode = newAdjListNode(src);
    if (graph->array[dest].head == NULL)
    {
        newNode->next = graph->array[dest].head;
        graph->array[dest].head = newNode;
    }
    else
    {
        check = graph->array[dest].head;
        while (check->next != NULL)
        {
            check = check->next;
        }
        check->next = newNode;
    }

    // newNode = newAdjListNode(src);
    // newNode->next = graph->array[dest].head;
    // graph->array[dest].head = newNode;
}

// A utility function to print the adjacency list
// representation of graph
void printGraph(struct Graph *graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode *pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

typedef struct
{
    int machineNumber;
    int operationTime;
} Item;

typedef struct
{

    int job_id;
    int machine_id;
    int duration;
    int level;
    int parent;
} Task;

int numMachines, numJobs;

void print_Task(Task task)
{
    printf("Task is level %d, job id %d machine id %d duration %d with parent %d \n", task.level, task.job_id, task.machine_id, task.duration, task.parent);
}

// void *myThreadFun(void *vargp)
void next_level(int level, Task list_tasks[], Task root_task[], int timerMachineArray[], int total_in_list, int parent, int jobid)
{

    // printf("level %d\n ", level);
    // printMatrixall(timerMachineArray);
    // print_list(list_tasks, total_in_list);

    for (int i = 0; i < numMachines * numJobs; i++)
    {

        if (root_task[i].level = (level - 1) && check_list(list_tasks, total_in_list, root_task[i]) == 0 && root_task[i].parent == parent)
        {

            int *timerMachineArray_temp;

            timerMachineArray_temp = malloc(numMachines * sizeof(int));

            list_tasks[total_in_list] = root_task[i];

            timerMachineArray_temp = timerMachineArray;

            updateMatrix(root_task[i].machine_id, timerMachineArray, root_task[i].duration);

            next_level(level, list_tasks, root_task, timerMachineArray, total_in_list, root_task[i].parent, root_task[i].job_id);

            timerMachineArray = timerMachineArray_temp;

            free(timerMachineArray_temp);
        }
        if (root_task[i].level = level && check_list(list_tasks, total_in_list, root_task[i]) == 0 && root_task[i].parent == jobid)
        {

            int *timerMachineArray_temp;

            timerMachineArray_temp = malloc(numMachines * sizeof(int));

            list_tasks[total_in_list] = root_task[i];

            timerMachineArray_temp = timerMachineArray;

            updateMatrix(root_task[i].machine_id, timerMachineArray, root_task[i].duration);

            next_level(level + 1, list_tasks, root_task, timerMachineArray, total_in_list + 1, root_task[i].parent, root_task[i].job_id);

            timerMachineArray = timerMachineArray_temp;

            free(timerMachineArray_temp);
        }
        // printMatrixall(timerMachineArray);
        // print_list(list_tasks,total_in_list);
    }
}
int check_list(Task list_tasks[], int total_in_list, Task task)
{

    for (int y = 0; y < total_in_list; y++)
    {
        if (list_tasks[y].job_id = task.job_id)
            return 1;
    }
    return 0;
}
int print_list(Task list_tasks[], int total_in_list)
{

    printf("---------------------------------------------------------\n");

    for (int y = 0; y < total_in_list; y++)
    {
        print_Task(list_tasks[y]);
    }

    printf("---------------------------------------------------------\n");
    return 0;
}

void updateMatrix(int i, int array[], int value)
{
    array[i] = array[i] + value;
}
void printMatrix(int i, int array[])
{
    printf("value in position %d of the array with %d \n", i, array[i]);
}
void printMatrixall(int array[])
{
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < numMachines; i++)
        printf("value in position %d of the array with %d \n", i, array[i]);
    printf("---------------------------------------------------------\n");
}

int main(int argc, char **argv)
{
    pthread_t thread_id;
    pthread_t tid;
    int totalOperationTime = 0;
    int startTime = 0;
    Task *root_task, *list_tasks;

    int i;
    int *timerMachineArray;

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

    // Allocate memory for array of structs
    root_task = malloc(numJobs * numMachines * sizeof(Task));
    list_tasks = malloc(numJobs * numMachines * sizeof(Task));
    timerMachineArray = malloc(numMachines * sizeof(int));

    struct Graph *graph = createGraph(numMachines);

    for (int i = 0; i < numMachines; i++)
    {
        timerMachineArray[i] = 0;
    }

    int level = 0;
    int parent_temp = 0;
    int job_number = 0;

    for (int i = 0; i < numMachines * numJobs; i++, level++)
    {
        if (level % numMachines == 0)
        {
            level = 0;
            parent_temp = 0;
        }
        root_task[i].level = level;
        root_task[i].job_id = i;
        root_task[i].parent = parent_temp;
        fscanf(file, "%d", &items[i].machineNumber);
        // printf("Machine %d ", items[i].machineNumber);
        root_task[i].machine_id = items[i].machineNumber;
        fscanf(file, "%d", &items[i].operationTime);
        // printf("Time %d \n", items[i].operationTime);
        root_task[i].duration = items[i].operationTime;
        parent_temp = items[i].machineNumber;
    }

    fclose(file);

    // Printing the read data
    printf("Number of Machines: %d\n", numMachines);
    printf("Number of Jobs: %d\n", numJobs);

    for (i = 0; i < numMachines * numJobs; i++)
    {
        if (root_task[i].level == 0)
        {
            // adiciona a task em que se esta
            list_tasks[0] = root_task[i];
            updateMatrix(root_task[i].machine_id, timerMachineArray, root_task[i].duration);

            next_level(1, list_tasks, root_task, timerMachineArray, 1, 0, root_task[i].job_id);

            updateMatrix(root_task[i].machine_id, timerMachineArray, 0);
        }
    }
    // pthread_join(thread_id, NULL);
    // printf("After Thread\n");
    /*
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
    */

    free(list_tasks);
    free(root_task);
    free(timerMachineArray);
    return 0;
}
