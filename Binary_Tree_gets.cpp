#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

struct TreeNode
{
    unsigned int data;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *root = NULL;

struct TreeNode *queue[100];
int front = 0;
int rear = 0;


/* =========================
   INPUT VALIDATION
   ========================= */

int isNumber(const char **str)
{
    if (**str == '\0')
        return 0;

    while (**str != '\0')
    {
        if (!isdigit((unsigned char)**str))
            return 0;

        (*str)++;
    }

    return 1;
}


int isValidUnsignedInt(const char **str)
{
    const char *temp = *str;
    char *endptr;
    unsigned long value;

    while (isspace((unsigned char)*temp))
        temp++;

    if (!isNumber(&temp))
        return 0;

    errno = 0;

    value = strtoul(*str, &endptr, 10);

    while (isspace((unsigned char)*endptr))
        endptr++;

    if (*endptr != '\0')
        return 0;

    if (errno == ERANGE || value > UINT_MAX)
        return 0;

    return 1;
}


unsigned int getUnsignedInt(const char *message)
{
    char input[100];
    const char *inputPtr = input;

    while (1)
    {
        printf("%s", *message);

        std::cin.getline(input, sizeof(input));

        inputPtr = input;

        if (isValidUnsignedInt(&inputPtr))
        {
            return (unsigned int)strtoul(input, NULL, 10);
        }

        printf("Invalid input! Please enter a valid unsigned integer.\n");
    }
}


/* =========================
   QUEUE FUNCTIONS
   ========================= */

void resetQueue()
{
    front = 0;
    rear = 0;
}


int isQueueEmpty()
{
    return front == rear;
}


int isQueueFull()
{
    return rear == 100;
}


void enqueue(struct TreeNode **node)
{
    if (isQueueFull())
    {
        printf("Queue is full!\n");
        return;
    }

    queue[rear++] = *node;
}


struct TreeNode *dequeue()
{
    if (isQueueEmpty())
        return NULL;

    return queue[front++];
}


/* =========================
   CREATE NODE
   ========================= */

struct TreeNode *createNode(unsigned int *data)
{
    struct TreeNode *newNode;

    newNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    (*newNode).data = *data;
    (*newNode).left = NULL;
    (*newNode).right = NULL;

    return newNode;
}


/* =========================
   SEARCH NODE
   ========================= */

struct TreeNode *findNode(struct TreeNode *node, unsigned int *data)
{
    struct TreeNode *result;

    if (node == NULL)
        return NULL;

    if ((*node).data == *data)
        return node;

    result = findNode((*node).left, data);

    if (result != NULL)
        return result;

    return findNode((*node).right, data);
}


/* =========================
   ADD NODE
   Binary Tree insertion
   using Level Order
   ========================= */

void addNode(unsigned int *data)
{
    struct TreeNode *newNode;
    struct TreeNode *current;

    newNode = createNode(data);

    if (newNode == NULL)
        return;

    if (root == NULL)
    {
        root = newNode;
        printf("Node added successfully.\n");
        return;
    }

    resetQueue();
    enqueue(&root);

    while (!isQueueEmpty())
    {
        current = dequeue();

        if ((*current).left == NULL)
        {
            (*current).left = newNode;
            printf("Node added successfully.\n");
            return;
        }
        else
        {
            enqueue(&((*current).left));
        }

        if ((*current).right == NULL)
        {
            (*current).right = newNode;
            printf("Node added successfully.\n");
            return;
        }
        else
        {
            enqueue(&((*current).right));
        }
    }
}


/* =========================
   TRAVERSALS
   ========================= */

void preorder(struct TreeNode *node)
{
    if (node == NULL)
        return;

    printf("%u ", (*node).data);

    preorder((*node).left);
    preorder((*node).right);
}


void inorder(struct TreeNode *node)
{
    if (node == NULL)
        return;

    inorder((*node).left);

    printf("%u ", (*node).data);

    inorder((*node).right);
}


void postorder(struct TreeNode *node)
{
    if (node == NULL)
        return;

    postorder((*node).left);
    postorder((*node).right);

    printf("%u ", (*node).data);
}


/* =========================
   LEVEL ORDER TRAVERSAL
   ========================= */

void levelOrder()
{
    struct TreeNode *current;

    if (root == NULL)
        return;

    resetQueue();

    enqueue(&root);

    while (!isQueueEmpty())
    {
        current = dequeue();

        printf("%u ", (*current).data);

        if ((*current).left != NULL)
            enqueue(&((*current).left));

        if ((*current).right != NULL)
            enqueue(&((*current).right));
    }
}


/* =========================
   DISPLAY TREE
   ========================= */

void displayTree()
{
    if (root == NULL)
    {
        printf("\nTree is empty!\n");
        return;
    }

    printf("\nPreorder    : ");
    preorder(root);

    printf("\nInorder     : ");
    inorder(root);

    printf("\nPostorder   : ");
    postorder(root);

    printf("\nLevel Order : ");
    levelOrder();

    printf("\n");
}


/* =========================
   UPDATE NODE
   ========================= */

void updateNode(unsigned int *oldData, unsigned int *newData)
{
    struct TreeNode *node;

    node = findNode(root, oldData);

    if (node == NULL)
    {
        printf("Node not found!\n");
        return;
    }

    (*node).data = *newData;

    printf("Node updated successfully.\n");
}


/* =========================
   FIND DEEPEST NODE
   ========================= */

struct TreeNode *findDeepestNode()
{
    struct TreeNode *current;
    struct TreeNode *deepest = NULL;

    if (root == NULL)
        return NULL;

    resetQueue();

    enqueue(&root);

    while (!isQueueEmpty())
    {
        current = dequeue();

        deepest = current;

        if ((*current).left != NULL)
            enqueue(&((*current).left));

        if ((*current).right != NULL)
            enqueue(&((*current).right));
    }

    return deepest;
}


/* =========================
   DELETE DEEPEST NODE
   ========================= */

void deleteDeepestNode(struct TreeNode *deepest)
{
    struct TreeNode *current;

    if (root == NULL || deepest == NULL)
        return;

    if (root == deepest)
    {
        free(root);
        root = NULL;
        return;
    }

    resetQueue();

    enqueue(&root);

    while (!isQueueEmpty())
    {
        current = dequeue();

        if ((*current).left != NULL)
        {
            if ((*current).left == deepest)
            {
                free((*current).left);
                (*current).left = NULL;
                return;
            }

            enqueue(&((*current).left));
        }

        if ((*current).right != NULL)
        {
            if ((*current).right == deepest)
            {
                free((*current).right);
                (*current).right = NULL;
                return;
            }

            enqueue(&((*current).right));
        }
    }
}


/* =========================
   DELETE NODE
   ========================= */

void deleteNode(unsigned int *data)
{
    struct TreeNode *target;
    struct TreeNode *deepest;

    target = findNode(root, data);

    if (target == NULL)
    {
        printf("Node not found!\n");
        return;
    }

    deepest = findDeepestNode();

    if (target == deepest)
    {
        deleteDeepestNode(deepest);
        printf("Node deleted successfully.\n");
        return;
    }

    (*target).data = (*deepest).data;

    deleteDeepestNode(deepest);

    printf("Node deleted successfully.\n");
}


/* =========================
   FREE TREE
   ========================= */

void freeTree(struct TreeNode *node)
{
    if (node == NULL)
        return;

    freeTree((*node).left);
    freeTree((*node).right);

    free(node);
}


/* =========================
   MAIN
   ========================= */

int main()
{
    char input[100];
    const char *inputPtr = input;

    unsigned int choice;
    unsigned int data;
    unsigned int oldData;
    unsigned int newData;

    while (1)
    {
        printf("\n==============================\n");
        printf("       BINARY TREE MENU\n");
        printf("==============================\n");

        printf("1. Add Node\n");
        printf("2. Display Traversals\n");
        printf("3. Search Node\n");
        printf("4. Update Node\n");
        printf("5. Delete Node\n");
        printf("6. Exit\n");

        printf("==============================\n");

        printf("Enter your choice: ");
        std::cin.getline(input, sizeof(input));

        inputPtr = input;

        if (!isValidUnsignedInt(&inputPtr))
        {
            printf("Invalid choice!\n");
            continue;
        }

        choice = (unsigned int)strtoul(input, NULL, 10);

        switch (choice)
        {
            case 1:

                data = getUnsignedInt("Enter data: ");

                addNode(&data);

                break;


            case 2:

                displayTree();

                break;


            case 3:

                data = getUnsignedInt("Enter data to search: ");

                if (findNode(root, &data) != NULL)
                    printf("Node found!\n");
                else
                    printf("Node not found!\n");

                break;


            case 4:

                oldData = getUnsignedInt("Enter old data: ");

                newData = getUnsignedInt("Enter new data: ");

                updateNode(&oldData, &newData);

                break;


            case 5:

                data = getUnsignedInt("Enter data to delete: ");

                deleteNode(&data);

                break;


            case 6:

                freeTree(root);
                root = NULL;

                printf("Program exited successfully.\n");

                return 0;


            default:

                printf("Invalid choice! Please select 1-6.\n");
        }
    }

    return 0;
}