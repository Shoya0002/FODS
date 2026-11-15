#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

struct TreeNode
{
    unsigned int data;
    struct TreeNode *left;
    struct TreeNode *right;
    int rightThread;
};

struct TreeNode *root = NULL;

// rightThread = 0 : Real right child or NULL
// rightThread = 1 : Inorder successor thread

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

// CREATE NODE

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
    (*newNode).rightThread = 0;

    return newNode;
}

// FIND INORDER SUCCESSOR

struct TreeNode *inorderSuccessor(struct TreeNode *node)
{
    struct TreeNode *current;

    if (node == NULL)
        return NULL;

    if ((*node).rightThread == 1)
        return (*node).right;

    current = (*node).right;

    while (current != NULL && (*current).left != NULL)
        current = (*current).left;

    return current;
}

// FIND MINIMUM NODE

struct TreeNode *findMinNode()
{
    struct TreeNode *current = root;

    if (current == NULL)
        return NULL;

    while ((*current).left != NULL)
        current = (*current).left;

    return current;
}

// SEARCH NODE

struct TreeNode *findNode(unsigned int *data)
{
    struct TreeNode *current = root;

    while (current != NULL)
    {
        if (*data == (*current).data)
            return current;

        if (*data < (*current).data)
        {
            current = (*current).left;
        }
        else
        {
            if ((*current).rightThread == 1)
                return NULL;

            current = (*current).right;
        }
    }

    return NULL;
}

// INSERT NODE

int insertNode(struct TreeNode *newNode)
{
    struct TreeNode *current = root;
    struct TreeNode *parent = NULL;

    if (root == NULL)
    {
        root = newNode;
        return 1;
    }

    while (current != NULL)
    {
        parent = current;

        if ((*newNode).data == (*current).data)
            return 0;

        if ((*newNode).data < (*current).data)
        {
            if ((*current).left == NULL)
                break;

            current = (*current).left;
        }
        else
        {
            if ((*current).rightThread == 1 ||
                (*current).right == NULL)
                break;

            current = (*current).right;
        }
    }

    // INSERT AS LEFT CHILD

    if ((*newNode).data < (*parent).data)
    {
        (*newNode).right = parent;
        (*newNode).rightThread = 1;

        (*parent).left = newNode;
    }

    // INSERT AS RIGHT CHILD

    else
    {
        (*newNode).right = (*parent).right;

        if ((*newNode).right != NULL)
            (*newNode).rightThread = 1;
        else
            (*newNode).rightThread = 0;

        (*parent).right = newNode;
        (*parent).rightThread = 0;
    }

    return 1;
}

void addNode(unsigned int *data)
{
    struct TreeNode *newNode;

    if (findNode(data) != NULL)
    {
        printf("Duplicate value! Node not added.\n");
        return;
    }

    newNode = createNode(data);

    if (newNode == NULL)
        return;

    if (insertNode(newNode))
        printf("Node added successfully.\n");
    else
    {
        free(newNode);
        printf("Duplicate value! Node not added.\n");
    }
}

// INORDER TRAVERSAL

void inorder()
{
    struct TreeNode *current = findMinNode();

    while (current != NULL)
    {
        printf("%u ", (*current).data);
        current = inorderSuccessor(current);
    }
}

// PREORDER TRAVERSAL

void preorder(struct TreeNode *node)
{
    if (node == NULL)
        return;

    printf("%u ", (*node).data);

    preorder((*node).left);

    if ((*node).rightThread == 0)
        preorder((*node).right);
}

// POSTORDER TRAVERSAL

void postorder(struct TreeNode *node)
{
    if (node == NULL)
        return;

    postorder((*node).left);

    if ((*node).rightThread == 0)
        postorder((*node).right);

    printf("%u ", (*node).data);
}

// LEVEL ORDER TRAVERSAL

void levelOrder()
{
    struct TreeNode **queue;
    struct TreeNode *current;

    size_t capacity = 16;
    size_t front = 0;
    size_t rear = 0;

    if (root == NULL)
        return;

    queue = (struct TreeNode **)malloc(capacity * sizeof(*queue));

    if (queue == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    queue[rear++] = root;

    while (front < rear)
    {
        current = queue[front++];

        printf("%u ", (*current).data);

        if ((*current).left != NULL)
        {
            if (rear == capacity)
            {
                size_t newCapacity = capacity * 2;

                struct TreeNode **temp =
                    (struct TreeNode **)realloc(queue, newCapacity * sizeof(*queue));

                if (temp == NULL)
                {
                    printf("\nQueue allocation failed!\n");
                    free(queue);
                    return;
                }

                queue = temp;
                capacity = newCapacity;
            }

            queue[rear++] = (*current).left;
        }

        if ((*current).rightThread == 0 &&
            (*current).right != NULL)
        {
            if (rear == capacity)
            {
                size_t newCapacity = capacity * 2;

                struct TreeNode **temp =
                    (struct TreeNode **)realloc(queue, newCapacity * sizeof(*queue));

                if (temp == NULL)
                {
                    printf("\nQueue allocation failed!\n");
                    free(queue);
                    return;
                }

                queue = temp;
                capacity = newCapacity;
            }

            queue[rear++] = (*current).right;
        }
    }

    free(queue);
}

// DISPLAY TREE

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
    inorder();

    printf("\nPostorder   : ");
    postorder(root);

    printf("\nLevel Order : ");
    levelOrder();

    printf("\n");
}

// DELETE NODE

void deleteNode(unsigned int *data)
{
    struct TreeNode *current = root;
    struct TreeNode *parent = NULL;

    // FIND NODE

    while (current != NULL)
    {
        if (*data == (*current).data)
            break;

        parent = current;

        if (*data < (*current).data)
        {
            current = (*current).left;
        }
        else
        {
            if ((*current).rightThread == 1)
                current = NULL;
            else
                current = (*current).right;
        }
    }

    if (current == NULL)
    {
        printf("Node not found!\n");
        return;
    }

    // CASE 1: TWO CHILDREN

    if ((*current).left != NULL &&
        (*current).rightThread == 0 &&
        (*current).right != NULL)
    {
        struct TreeNode *successorParent = current;
        struct TreeNode *successor = (*current).right;

        while ((*successor).left != NULL)
        {
            successorParent = successor;
            successor = (*successor).left;
        }

        (*current).data = (*successor).data;

        parent = successorParent;
        current = successor;
    }

    // CASE 2: ONLY LEFT CHILD

    if ((*current).left != NULL &&
        ((*current).rightThread == 1 ||
         (*current).right == NULL))
    {
        struct TreeNode *child = (*current).left;
        struct TreeNode *rightmost = child;

        while ((*rightmost).rightThread == 0 &&
               (*rightmost).right != NULL)
        {
            rightmost = (*rightmost).right;
        }

        // RECONNECT PARENT

        if (parent == NULL)
        {
            root = child;
        }
        else if ((*parent).left == current)
        {
            (*parent).left = child;
        }
        else
        {
            (*parent).right = child;
            (*parent).rightThread = 0;
        }

        // RECONNECT THREAD

        (*rightmost).right = (*current).right;

        if ((*rightmost).right != NULL)
            (*rightmost).rightThread = 1;
        else
            (*rightmost).rightThread = 0;

        free(current);

        printf("Node deleted successfully.\n");
        return;
    }

    // CASE 3: ONLY RIGHT CHILD

    if ((*current).left == NULL &&
        (*current).rightThread == 0 &&
        (*current).right != NULL)
    {
        struct TreeNode *child = (*current).right;

        // RECONNECT PARENT

        if (parent == NULL)
        {
            root = child;
        }
        else if ((*parent).left == current)
        {
            (*parent).left = child;
        }
        else
        {
            (*parent).right = child;
            (*parent).rightThread = 0;
        }

        free(current);

        printf("Node deleted successfully.\n");
        return;
    }

    // CASE 4: LEAF NODE

    if ((*current).left == NULL &&
        ((*current).rightThread == 1 ||
         (*current).right == NULL))
    {
        if (parent == NULL)
        {
            root = NULL;
        }
        else if ((*parent).left == current)
        {
            (*parent).left = NULL;
        }
        else
        {
            // RESTORE PARENT'S THREAD

            (*parent).right = (*current).right;

            if ((*parent).right != NULL)
                (*parent).rightThread = 1;
            else
                (*parent).rightThread = 0;
        }

        free(current);

        printf("Node deleted successfully.\n");
        return;
    }
}

// UPDATE NODE

void updateNode(unsigned int *oldData, unsigned int *newData)
{
    struct TreeNode *newNode;

    if (findNode(oldData) == NULL)
    {
        printf("Old node not found!\n");
        return;
    }

    if (*oldData == *newData)
    {
        printf("Old and new values are same.\n");
        return;
    }

    if (findNode(newData) != NULL)
    {
        printf("New value already exists!\n");
        return;
    }

    // ALLOCATE BEFORE DELETING OLD NODE

    newNode = createNode(newData);

    if (newNode == NULL)
        return;

    deleteNode(oldData);

    if (insertNode(newNode))
        printf("Node updated successfully.\n");
    else
    {
        free(newNode);
        printf("Update failed!\n");
    }
}

// FREE TREE

void freeTree(struct TreeNode *node)
{
    if (node == NULL)
        return;

    freeTree((*node).left);

    if ((*node).rightThread == 0)
        freeTree((*node).right);

    free(node);
}

// MAIN

int main()
{
    unsigned int choice;
    unsigned int data;
    unsigned int oldData;
    unsigned int newData;

    while (1)
    {
        printf("\n==============================\n");
        printf(" THREADED BINARY SEARCH TREE\n");
        printf("==============================\n");

        printf("1. Add Node\n");
        printf("2. Display Traversals\n");
        printf("3. Search Node\n");
        printf("4. Update Node\n");
        printf("5. Delete Node\n");
        printf("6. Exit\n");

        printf("==============================\n");

        choice = getUnsignedInt("Enter your choice: ");

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

            if (findNode(&data) != NULL)
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
}
