#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#define MAX 100

unsigned int vertices[MAX];
int graph[MAX][MAX];
int vertexCount = 0;

int stack[MAX];
int top = -1;

int queue[MAX];
int front = 0;
int rear = 0;

// INPUT VALIDATION

int isValidUnsignedInt(const char *str)
{
    char *endptr;
    unsigned long value;

    while (isspace((unsigned char)*str))
        str++;

    if (!isdigit((unsigned char)*str))
        return 0;

    errno = 0;
    value = strtoul(str, &endptr, 10);

    if (errno == ERANGE || value > UINT_MAX)
        return 0;

    while (isspace((unsigned char)*endptr))
        endptr++;

    return *endptr == '\0';
}

unsigned int getUnsignedInt(const char *message)
{
    char input[100];

    while (1)
    {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
            exit(0);

        if (strchr(input, '\n') == NULL && !feof(stdin))
        {
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            printf("Invalid input!\n");
            continue;
        }

        if (isValidUnsignedInt(input))
            return (unsigned int)strtoul(input, NULL, 10);

        printf("Invalid input! Enter an unsigned integer.\n");
    }
}

// STACK OPERATIONS

void push(int index)
{
    if (top == MAX - 1)
    {
        printf("Stack Overflow!\n");
        return;
    }

    stack[++top] = index;
}

int pop()
{
    if (top == -1)
        return -1;

    return stack[top--];
}

// QUEUE OPERATIONS

void enqueue(int index)
{
    if (rear == MAX)
    {
        printf("Queue Overflow!\n");
        return;
    }

    queue[rear++] = index;
}

int dequeue()
{
    if (front == rear)
        return -1;

    return queue[front++];
}

// FIND VERTEX

int findVertex(unsigned int data)
{
    int i;

    for (i = 0; i < vertexCount; i++)
    {
        if (vertices[i] == data)
            return i;
    }

    return -1;
}

// ADD VERTEX

void addVertex(unsigned int data)
{
    int i;

    if (vertexCount == MAX)
    {
        printf("Graph is full!\n");
        return;
    }

    if (findVertex(data) != -1)
    {
        printf("Duplicate vertex! Vertex not added.\n");
        return;
    }

    vertices[vertexCount] = data;

    for (i = 0; i <= vertexCount; i++)
    {
        graph[vertexCount][i] = 0;
        graph[i][vertexCount] = 0;
    }

    vertexCount++;

    printf("Vertex added successfully.\n");
}

// ADD EDGE

void addEdge(unsigned int source, unsigned int destination)
{
    int i = findVertex(source);
    int j = findVertex(destination);

    if (i == -1 || j == -1)
    {
        printf("Vertex does not exist! Edge not added.\n");
        return;
    }

    if (i == j)
    {
        printf("Self-loop is not allowed!\n");
        return;
    }

    if (graph[i][j] == 1)
    {
        printf("Edge already exists!\n");
        return;
    }

    graph[i][j] = 1;
    graph[j][i] = 1;

    printf("Edge added successfully.\n");
}

// CHECK CONNECTIVITY

int isConnected()
{
    int visited[MAX] = {0};
    int changed = 1;
    int i, j;

    if (vertexCount == 0)
        return 0;

    visited[0] = 1;

    while (changed == 1)
    {
        changed = 0;

        for (i = 0; i < vertexCount; i++)
        {
            if (visited[i] == 1)
            {
                for (j = 0; j < vertexCount; j++)
                {
                    if (graph[i][j] == 1 && visited[j] == 0)
                    {
                        visited[j] = 1;
                        changed = 1;
                    }
                }
            }
        }
    }

    for (i = 0; i < vertexCount; i++)
    {
        if (visited[i] == 0)
            return 0;
    }

    return 1;
}

// DISPLAY GRAPH

void displayGraph()
{
    int i, j;

    if (vertexCount == 0)
    {
        printf("Graph is empty!\n");
        return;
    }

    if (isConnected() == 0)
    {
        printf("Graph does not exist! Graph is disconnected.\n");
        return;
    }

    printf("\nAdjacency Matrix:\n\n");

    printf("%8s", "");

    for (i = 0; i < vertexCount; i++)
        printf("%8u", vertices[i]);

    printf("\n");

    for (i = 0; i < vertexCount; i++)
    {
        printf("%8u", vertices[i]);

        for (j = 0; j < vertexCount; j++)
            printf("%8d", graph[i][j]);

        printf("\n");
    }

    printf("\nAdjacency List Representation:\n");

    for (i = 0; i < vertexCount; i++)
    {
        printf("%u : ", vertices[i]);

        for (j = 0; j < vertexCount; j++)
        {
            if (graph[i][j] == 1)
                printf("%u ", vertices[j]);
        }

        printf("\n");
    }
}

// SEARCH VERTEX

void searchVertex(unsigned int data)
{
    if (findVertex(data) == -1)
        printf("Vertex not found!\n");
    else
        printf("Vertex found!\n");
}

// UPDATE VERTEX

void updateVertex(unsigned int oldData, unsigned int newData)
{
    int index = findVertex(oldData);

    if (index == -1)
    {
        printf("Old vertex not found!\n");
        return;
    }

    if (oldData == newData)
    {
        printf("Old and new values are same.\n");
        return;
    }

    if (findVertex(newData) != -1)
    {
        printf("New vertex already exists!\n");
        return;
    }

    vertices[index] = newData;

    printf("Vertex updated successfully.\n");
}

// DELETE EDGE

void deleteEdge(unsigned int source, unsigned int destination)
{
    int i = findVertex(source);
    int j = findVertex(destination);

    if (i == -1 || j == -1)
    {
        printf("Vertex does not exist!\n");
        return;
    }

    if (graph[i][j] == 0)
    {
        printf("Edge does not exist!\n");
        return;
    }

    graph[i][j] = 0;
    graph[j][i] = 0;

    printf("Edge deleted successfully.\n");
}

// DELETE VERTEX

void deleteVertex(unsigned int data)
{
    int index = findVertex(data);
    int i, j;

    if (index == -1)
    {
        printf("Vertex not found!\n");
        return;
    }

    // Shift vertices left.

    for (i = index; i < vertexCount - 1; i++)
        vertices[i] = vertices[i + 1];

    // Shift matrix rows upward.

    for (i = index; i < vertexCount - 1; i++)
    {
        for (j = 0; j < vertexCount; j++)
            graph[i][j] = graph[i + 1][j];
    }

    // Shift matrix columns left.

    for (j = index; j < vertexCount - 1; j++)
    {
        for (i = 0; i < vertexCount - 1; i++)
            graph[i][j] = graph[i][j + 1];
    }

    vertexCount--;

    // Clear unused row and column.

    for (i = 0; i <= vertexCount; i++)
    {
        graph[vertexCount][i] = 0;
        graph[i][vertexCount] = 0;
    }

    printf("Vertex deleted successfully.\n");
}

// BFS TRAVERSAL - ITERATIVE

void BFS(unsigned int start)
{
    int visited[MAX] = {0};
    int startIndex = findVertex(start);
    int current, i;

    if (startIndex == -1)
    {
        printf("Starting vertex does not exist!\n");
        return;
    }

    if (isConnected() == 0)
    {
        printf("Graph does not exist! Graph is disconnected.\n");
        return;
    }

    front = 0;
    rear = 0;

    visited[startIndex] = 1;
    enqueue(startIndex);

    printf("BFS Traversal: ");

    while (front < rear)
    {
        current = dequeue();

        printf("%u ", vertices[current]);

        for (i = 0; i < vertexCount; i++)
        {
            if (graph[current][i] == 1 && visited[i] == 0)
            {
                visited[i] = 1;
                enqueue(i);
            }
        }
    }

    printf("\n");
}

// DFS TRAVERSAL - ITERATIVE

void DFS(unsigned int start)
{
    int visited[MAX] = {0};
    int nextNeighbor[MAX] = {0};

    int startIndex = findVertex(start);
    int current, i, found;

    if (startIndex == -1)
    {
        printf("Starting vertex does not exist!\n");
        return;
    }

    if (isConnected() == 0)
    {
        printf("Graph does not exist! Graph is disconnected.\n");
        return;
    }

    top = -1;

    push(startIndex);
    visited[startIndex] = 1; 

    printf("DFS Traversal: %u ", vertices[startIndex]);

    while (top != -1)
    {
        current = stack[top];
        found = 0;

        for (i = nextNeighbor[current]; i < vertexCount; i++)
        {
            nextNeighbor[current] = i + 1;

            if (graph[current][i] == 1 && visited[i] == 0)
            {
                visited[i] = 1;

                printf("%u ", vertices[i]);

                push(i);

                found = 1;
                break;
            }
        }

        if (found == 0)
            pop();
    }

    printf("\n");
}

// TRAVERSAL SUBMENU

void traversalMenu()
{
    unsigned int choice;
    unsigned int start;

    while (1)
    {
        printf("\n========== GRAPH TRAVERSAL ==========\n");
        printf("1. BFS Traversal\n");
        printf("2. DFS Traversal\n");
        printf("3. Back to Main Menu\n");
        printf("=====================================\n");

        choice = getUnsignedInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            if (vertexCount == 0)
            {
                printf("Graph is empty!\n");
                break;
            }

            if (isConnected() == 0)
            {
                printf("Graph does not exist! Graph is disconnected.\n");
                break;
            }

            start = getUnsignedInt("Enter starting vertex for BFS: ");
            BFS(start);
            break;

        case 2:
            if (vertexCount == 0)
            {
                printf("Graph is empty!\n");
                break;
            }

            if (isConnected() == 0)
            {
                printf("Graph does not exist! Graph is disconnected.\n");
                break;
            }

            start = getUnsignedInt("Enter starting vertex for DFS: ");
            DFS(start);
            break;

        case 3:
            return;

        default:
            printf("Invalid choice! Select 1-3.\n");
        }
    }
}

// MAIN FUNCTION

int main()
{
    unsigned int choice;
    unsigned int data;
    unsigned int oldData, newData;
    unsigned int source, destination;

    while (1)
    {
        printf("\n================================\n");
        printf("       GRAPH USING ARRAYS\n");
        printf("================================\n");

        printf("1. Add Vertex\n");
        printf("2. Add Edge\n");
        printf("3. Display Graph\n");
        printf("4. Search Vertex\n");
        printf("5. Update Vertex\n");
        printf("6. Delete Vertex\n");
        printf("7. Delete Edge\n");
        printf("8. Traversal\n");
        printf("9. Exit\n");

        printf("================================\n");

        choice = getUnsignedInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            data = getUnsignedInt("Enter vertex: ");
            addVertex(data);
            break;

        case 2:
            source = getUnsignedInt("Enter source vertex: ");
            destination = getUnsignedInt("Enter destination vertex: ");
            addEdge(source, destination);
            break;

        case 3:
            displayGraph();
            break;

        case 4:
            data = getUnsignedInt("Enter vertex to search: ");
            searchVertex(data);
            break;

        case 5:
            oldData = getUnsignedInt("Enter old vertex: ");
            newData = getUnsignedInt("Enter new vertex: ");
            updateVertex(oldData, newData);
            break;

        case 6:
            data = getUnsignedInt("Enter vertex to delete: ");
            deleteVertex(data);
            break;

        case 7:
            source = getUnsignedInt("Enter source vertex: ");
            destination = getUnsignedInt("Enter destination vertex: ");
            deleteEdge(source, destination);
            break;

        case 8:
            traversalMenu();
            break;

        case 9:
            printf("Program exited successfully.\n");
            return 0;

        default:
            printf("Invalid choice! Please select 1-9.\n");
        }
    }
}
    