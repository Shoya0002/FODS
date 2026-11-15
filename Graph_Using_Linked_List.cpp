#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

typedef struct Vertex Vertex;
typedef struct Edge Edge;

struct Edge
{
    Vertex *vertex;
    Edge *next;
};

struct Vertex
{
    unsigned int data;
    Edge *edges;
    Vertex *next;
    int visited;
};

Vertex *head = NULL;
int vertexCount = 0;

unsigned int getUnsignedInt(const char *message)
{
    char input[100];
    char *p, *end;
    unsigned long value;
    int ch;

    while (1)
    {
        printf("%s", message);
        if (fgets(input, sizeof(input), stdin) == NULL)
            exit(0);
        if (strchr(input, '\n') == NULL && !feof(stdin))
        {
            while ((ch = getchar()) != '\n' && ch != EOF)
            {
            }
            printf("Invalid input!\n");
            continue;
        }
        p = input;
        while (isspace((unsigned char)*p))
            p++;
        if (!isdigit((unsigned char)*p))
        {
            printf("Invalid input! Enter an unsigned integer.\n");
            continue;
        }
        errno = 0;
        value = strtoul(p, &end, 10);
        while (isspace((unsigned char)*end))
            end++;
        if (errno == ERANGE || value > UINT_MAX || *end != '\0')
        {
            printf("Invalid input! Enter an unsigned integer.\n");
            continue;
        }
        return (unsigned int)value;
    }
}

Vertex *findVertex(unsigned int data)
{
    Vertex *current = head;
    while (current != NULL)
    {
        if (current->data == data)
            return current;
        current = current->next;
    }
    return NULL;
}

Edge *findEdge(Vertex *source, Vertex *destination)
{
    Edge *current = source->edges;
    while (current != NULL)
    {
        if (current->vertex == destination)
            return current;
        current = current->next;
    }
    return NULL;
}

void addVertex(unsigned int data)
{
    Vertex *node, *current;
    if (findVertex(data) != NULL)
    {
        printf("Duplicate vertex! Vertex not added.\n");
        return;
    }
    node = (Vertex *)malloc(sizeof(Vertex));
    if (node == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    node->data = data;
    node->edges = NULL;
    node->next = NULL;
    node->visited = 0;
    if (head == NULL)
        head = node;
    else
    {
        current = head;
        while (current->next != NULL)
            current = current->next;
        current->next = node;
    }
    vertexCount++;
    printf("Vertex added successfully.\n");
}

/* Append a neighbour so traversal order follows edge insertion order. */
void appendEdge(Vertex *source, Edge *newEdge)
{
    Edge *current;
    newEdge->next = NULL;
    if (source->edges == NULL)
        source->edges = newEdge;
    else
    {
        current = source->edges;
        while (current->next != NULL)
            current = current->next;
        current->next = newEdge;
    }
}

void addEdge(unsigned int source, unsigned int destination)
{
    Vertex *a = findVertex(source), *b = findVertex(destination);
    Edge *ab, *ba;
    if (a == NULL || b == NULL)
    {
        printf("Vertex does not exist! Edge not added.\n");
        return;
    }
    if (a == b)
    {
        printf("Self-loop is not allowed!\n");
        return;
    }
    if (findEdge(a, b) != NULL)
    {
        printf("Edge already exists!\n");
        return;
    }
    ab = (Edge *)malloc(sizeof(Edge));
    ba = (Edge *)malloc(sizeof(Edge));
    if (ab == NULL || ba == NULL)
    {
        free(ab);
        free(ba);
        printf("Memory allocation failed!\n");
        return;
    }
    ab->vertex = b;
    ba->vertex = a;
    appendEdge(a, ab);
    appendEdge(b, ba);
    printf("Edge added successfully.\n");
}

void resetVisited(void)
{
    Vertex *v = head;
    while (v != NULL)
    {
        v->visited = 0;
        v = v->next;
    }
}

int isConnected(void)
{
    Vertex **queue, *v;
    Edge *e;
    int front = 0, rear = 0, count = 0;
    if (head == NULL)
        return 0;
    queue = (Vertex **)malloc((size_t)vertexCount * sizeof(Vertex *));
    if (queue == NULL)
    {
        printf("Memory allocation failed!\n");
        return 0;
    }
    resetVisited();
    head->visited = 1;
    queue[rear++] = head;
    while (front < rear)
    {
        v = queue[front++];
        count++;
        for (e = v->edges; e != NULL; e = e->next)
        {
            if (!e->vertex->visited)
            {
                e->vertex->visited = 1;
                queue[rear++] = e->vertex;
            }
        }
    }
    free(queue);
    resetVisited();
    return count == vertexCount;
}

void displayGraph(void)
{
    Vertex *v;
    Edge *e;
    if (head == NULL)
    {
        printf("Graph is empty!\n");
        return;
    }
    if (!isConnected())
    {
        printf("Graph does not exist! Graph is disconnected.\n");
        return;
    }
    printf("\nAdjacency List (Linked List Representation):\n");
    for (v = head; v != NULL; v = v->next)
    {
        printf("%u -> ", v->data);
        for (e = v->edges; e != NULL; e = e->next)
            printf("%u -> ", e->vertex->data);
        printf("NULL\n");
    }
}

void searchVertex(unsigned int data)
{
    printf(findVertex(data) ? "Vertex found!\n" : "Vertex not found!\n");
}

void updateVertex(unsigned int oldData, unsigned int newData)
{
    Vertex *v = findVertex(oldData);
    if (v == NULL)
    {
        printf("Old vertex not found!\n");
        return;
    }
    if (oldData == newData)
    {
        printf("Old and new values are same.\n");
        return;
    }
    if (findVertex(newData) != NULL)
    {
        printf("New vertex already exists!\n");
        return;
    }
    v->data = newData;
    printf("Vertex updated successfully.\n");
}

void removeDirectedEdge(Vertex *source, Vertex *destination)
{
    Edge *current = source->edges, *previous = NULL;
    while (current != NULL)
    {
        if (current->vertex == destination)
        {
            if (previous == NULL)
                source->edges = current->next;
            else
                previous->next = current->next;
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void deleteEdge(unsigned int source, unsigned int destination)
{
    Vertex *a = findVertex(source), *b = findVertex(destination);
    if (a == NULL || b == NULL)
    {
        printf("Vertex does not exist!\n");
        return;
    }
    if (findEdge(a, b) == NULL)
    {
        printf("Edge does not exist!\n");
        return;
    }
    removeDirectedEdge(a, b);
    removeDirectedEdge(b, a);
    printf("Edge deleted successfully.\n");
}

void deleteVertex(unsigned int data)
{
    Vertex *current = head, *previous = NULL, *other;
    Edge *e, *nextEdge;
    while (current != NULL && current->data != data)
    {
        previous = current;
        current = current->next;
    }
    if (current == NULL)
    {
        printf("Vertex not found!\n");
        return;
    }
    /* Remove every other vertex's link to the deleted vertex. */
    for (other = head; other != NULL; other = other->next)
    {
        if (other != current)
            removeDirectedEdge(other, current);
    }
    /* Free the deleted vertex's own adjacency list. */
    e = current->edges;
    while (e != NULL)
    {
        nextEdge = e->next;
        free(e);
        e = nextEdge;
    }
    if (previous == NULL)
        head = current->next;
    else
        previous->next = current->next;
    free(current);
    vertexCount--;
    printf("Vertex deleted successfully.\n");
}

void BFS(unsigned int start)
{
    Vertex *s = findVertex(start), *v, **queue;
    Edge *e;
    int front = 0, rear = 0;
    if (s == NULL)
    {
        printf("Starting vertex not found!\n");
        return;
    }
    queue = (Vertex **)malloc((size_t)vertexCount * sizeof(Vertex *));
    if (queue == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    resetVisited();
    s->visited = 1;
    queue[rear++] = s;
    printf("BFS Traversal: ");
    while (front < rear)
    {
        v = queue[front++];
        printf("%u ", v->data);
        for (e = v->edges; e != NULL; e = e->next)
        {
            if (!e->vertex->visited)
            {
                e->vertex->visited = 1;
                queue[rear++] = e->vertex;
            }
        }
    }
    printf("\n");
    free(queue);
    resetVisited();
}

typedef struct
{
    Vertex *vertex;
    Edge *nextNeighbor;
} DFSFrame;

void DFS(unsigned int start)
{
    Vertex *s = findVertex(start);
    DFSFrame *stack;
    Edge *e;
    int top = 0;
    if (s == NULL)
    {
        printf("Starting vertex not found!\n");
        return;
    }
    stack = (DFSFrame *)malloc((size_t)vertexCount * sizeof(DFSFrame));
    if (stack == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    resetVisited();
    s->visited = 1;
    stack[0].vertex = s;
    stack[0].nextNeighbor = s->edges;
    printf("DFS Traversal: %u ", s->data);
    while (top >= 0)
    {
        /* Remember where the current vertex stopped scanning. */
        e = stack[top].nextNeighbor;
        while (e != NULL && e->vertex->visited)
            e = e->next;
        if (e == NULL)
        {
            top--; /* No unvisited neighbour: backtrack. */
        }
        else
        {
            stack[top].nextNeighbor = e->next;
            e->vertex->visited = 1;
            printf("%u ", e->vertex->data);
            top++;
            stack[top].vertex = e->vertex;
            stack[top].nextNeighbor = e->vertex->edges;
        }
    }
    printf("\n");
    free(stack);
    resetVisited();
}

void traversalMenu(void)
{
    unsigned int choice, start;
    while (1)
    {
        printf("\n========== GRAPH TRAVERSAL ==========\n");
        printf("1. BFS Traversal\n2. DFS Traversal\n3. Back to Main Menu\n");
        printf("=====================================\n");
        choice = getUnsignedInt("Enter your choice: ");
        if (choice == 3)
            return;
        if (choice != 1 && choice != 2)
        {
            printf("Invalid choice! Select 1-3.\n");
            continue;
        }
        if (head == NULL)
        {
            printf("Graph is empty!\n");
            continue;
        }
        if (!isConnected())
        {
            printf("Graph does not exist! Graph is disconnected.\n");
            continue;
        }
        start = getUnsignedInt(choice == 1 ? "Enter starting vertex for BFS: " : "Enter starting vertex for DFS: ");
        if (choice == 1)
            BFS(start);
        else
            DFS(start);
    }
}

void freeGraph(void)
{
    Vertex *v = head, *nextVertex;
    Edge *e, *nextEdge;
    while (v != NULL)
    {
        nextVertex = v->next;
        e = v->edges;
        while (e != NULL)
        {
            nextEdge = e->next;
            free(e);
            e = nextEdge;
        }
        free(v);
        v = nextVertex;
    }
    head = NULL;
    vertexCount = 0;
}

int main(void)
{
    unsigned int choice, data, oldData, newData, source, destination;
    while (1)
    {
        printf("\n================================\n");
        printf("     GRAPH USING LINKED LIST\n");
        printf("================================\n");
        printf("1. Add Vertex\n2. Add Edge\n3. Display Graph\n");
        printf("4. Search Vertex\n5. Update Vertex\n6. Delete Vertex\n");
        printf("7. Delete Edge\n8. Traversal\n9. Exit\n");
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
            freeGraph();
            printf("Program exited successfully.\n");
            return 0;
        default:
            printf("Invalid choice! Please select 1-9.\n");
        }
    }
}
