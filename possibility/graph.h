#ifndef GRAPH_FILE
#define GRAPH_FILE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

// from https://www.geeksforgeeks.org/graph-and-its-representations/
// A structure to represent an adjacency list node
struct AdjListNode
{
    int jobs_id;
    int duration;
    int level;
    int machine;
    struct AdjListNode *next;
};

// A structure to represent an adjacency list
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
struct AdjListNode *newAdjListNode(int jobs_id, int duration, int level, int machine)
{
    struct AdjListNode *newNode = (struct AdjListNode *)malloc(
        sizeof(struct AdjListNode));
    newNode->jobs_id = jobs_id;
    newNode->duration = duration;
    newNode->level = level;
    newNode->machine = machine;
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
void addEdge(struct Graph *graph, int job_id_src, int job_id_dest, int duration, int level, int machine)
{
    // Add an edge from job_id_src to job_id_dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the beginning
    struct AdjListNode *check = NULL;
    struct AdjListNode *newNode = newAdjListNode(job_id_dest, duration, level, machine);

    if (graph->array[job_id_src].head == NULL)
    {
        newNode->next = graph->array[job_id_src].head;
        graph->array[job_id_src].head = newNode;
    }
    else
    {

        check = graph->array[job_id_src].head;
        while (check->next != NULL)
        {
            check = check->next;
        }
        // graph->array[src].head = newNode;
        check->next = newNode;
    }

    // Since graph is undirected, add an edge from
    // dest to src also
    /*newNode = newAdjListNode(src, duration, jobs_id, machine);
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
    }*/
}

// A utility function to print the adjacency list
// representation of graph
void printGraph(struct Graph *graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode *pCrawl = graph->array[v].head;
        printf("\nAdjacency list of vertex %d\nhead \n", v);
        while (pCrawl)
        {
            printf("jobs_id-> %d level -> %d machine -> %d duration -> %d \n", pCrawl->jobs_id, pCrawl->level, pCrawl->machine, pCrawl->duration);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

void deleteGraph(struct Graph *graph)
{
    if (graph == NULL)
        return;

    // Free memory for each adjacency list
    for (int i = 0; i < graph->V; ++i)
    {
        struct AdjListNode *current = graph->array[i].head;
        while (current != NULL)
        {
            struct AdjListNode *temp = current;
            current = current->next;
            free(temp);
        }
    }

    // Free memory for the array of adjacency lists
    free(graph->array);

    // Free memory for the graph structure
    free(graph);
}

#endif