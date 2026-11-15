#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

/* NODE STRUCTURE */

struct TreeNode
{
    unsigned int data;
    int height;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
};

struct TreeNode *root = NULL;

/* STACK STRUCTURE */

struct StackNode
{
    struct TreeNode *tree;
    struct StackNode *next;
};

struct Stack
{
    struct StackNode *top;
};

/* QUEUE STRUCTURE */

struct QueueNode
{
    struct TreeNode *tree;
    struct QueueNode *next;
};

struct Queue
{
    struct QueueNode *front;
    struct QueueNode *rear;
};

/* INPUT USING gets() */

unsigned int getUnsignedInt(const char *message)
{
    char input[100];
    char *start;
    char *endptr;
    unsigned long value;
    int valid;
    int i;

    while (1)
    {
        printf("%s", message);

        if (!std::cin.getline(input, sizeof(input)))
        {
            printf("\nInput ended.\n");
            exit(0);
        }

        start = input;

        while (isspace((unsigned char)*start))
            start++;

        if (*start == '\0')
        {
            printf("Invalid input! Enter a number.\n");
            continue;
        }

        valid = 1;

        for (i = 0; start[i] != '\0'; i++)
        {
            if (!isdigit((unsigned char)start[i]))
            {
                valid = 0;
                break;
            }
        }

        if (valid == 0)
        {
            printf("Invalid input! Enter an unsigned integer.\n");
            continue;
        }

        errno = 0;
        value = strtoul(start, &endptr, 10);

        if (errno == ERANGE || value > UINT_MAX)
        {
            printf("Number is too large!\n");
            continue;
        }

        return (unsigned int)value;
    }
}

/* CREATE NODE */

struct TreeNode *createNode(unsigned int data)
{
    struct TreeNode *newNode;

    newNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->height = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    return newNode;
}

/* HEIGHT AND BALANCE */

int getHeight(struct TreeNode *node)
{
    if (node == NULL)
        return 0;

    return node->height;
}

int max(int a, int b)
{
    if (a > b)
        return a;

    return b;
}

void updateHeight(struct TreeNode *node)
{
    if (node != NULL)
    {
        node->height = 1 + max(
            getHeight(node->left),
            getHeight(node->right)
        );
    }
}

int getBalance(struct TreeNode *node)
{
    if (node == NULL)
        return 0;

    return getHeight(node->left) -
           getHeight(node->right);
}

/* STACK FUNCTIONS */

void initStack(struct Stack *s)
{
    s->top = NULL;
}

void push(struct Stack *s, struct TreeNode *tree)
{
    struct StackNode *newNode;

    newNode = (struct StackNode *)malloc(sizeof(struct StackNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    newNode->tree = tree;
    newNode->next = s->top;
    s->top = newNode;
}

struct TreeNode *pop(struct Stack *s)
{
    struct StackNode *temp;
    struct TreeNode *tree;

    if (s->top == NULL)
        return NULL;

    temp = s->top;
    tree = temp->tree;

    s->top = temp->next;

    free(temp);

    return tree;
}

int isStackEmpty(struct Stack *s)
{
    return s->top == NULL;
}

void destroyStack(struct Stack *s)
{
    while (!isStackEmpty(s))
        pop(s);
}

/* QUEUE FUNCTIONS */

void enqueue(struct Queue *q, struct TreeNode *tree)
{
    struct QueueNode *newNode;

    newNode = (struct QueueNode *)malloc(sizeof(struct QueueNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    newNode->tree = tree;
    newNode->next = NULL;

    if (q->rear == NULL)
    {
        q->front = newNode;
        q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

struct TreeNode *dequeue(struct Queue *q)
{
    struct QueueNode *temp;
    struct TreeNode *tree;

    if (q->front == NULL)
        return NULL;

    temp = q->front;
    tree = temp->tree;

    q->front = temp->next;

    if (q->front == NULL)
        q->rear = NULL;

    free(temp);

    return tree;
}

/* RIGHT ROTATION */

struct TreeNode *rightRotate(struct TreeNode *y)
{
    struct TreeNode *x;
    struct TreeNode *temp;
    struct TreeNode *parent;

    x = y->left;
    temp = x->right;
    parent = y->parent;

    x->right = y;
    y->parent = x;

    y->left = temp;

    if (temp != NULL)
        temp->parent = y;

    x->parent = parent;

    if (parent == NULL)
        root = x;
    else if (parent->left == y)
        parent->left = x;
    else
        parent->right = x;

    updateHeight(y);
    updateHeight(x);

    return x;
}

/* LEFT ROTATION */

struct TreeNode *leftRotate(struct TreeNode *x)
{
    struct TreeNode *y;
    struct TreeNode *temp;
    struct TreeNode *parent;

    y = x->right;
    temp = y->left;
    parent = x->parent;

    y->left = x;
    x->parent = y;

    x->right = temp;

    if (temp != NULL)
        temp->parent = x;

    y->parent = parent;

    if (parent == NULL)
        root = y;
    else if (parent->left == x)
        parent->left = y;
    else
        parent->right = y;

    updateHeight(x);
    updateHeight(y);

    return y;
}

/* ITERATIVE REBALANCING */

void rebalanceUpward(struct TreeNode *node)
{
    int balance;

    while (node != NULL)
    {
        updateHeight(node);
        balance = getBalance(node);

        if (balance > 1)
        {
            if (getBalance(node->left) < 0)
                leftRotate(node->left);

            node = rightRotate(node);
        }
        else if (balance < -1)
        {
            if (getBalance(node->right) > 0)
                rightRotate(node->right);

            node = leftRotate(node);
        }

        node = node->parent;
    }
}

/* INSERT NODE ITERATIVELY */

void addNode(unsigned int data)
{
    struct TreeNode *current;
    struct TreeNode *parent;
    struct TreeNode *newNode;

    current = root;
    parent = NULL;

    while (current != NULL)
    {
        parent = current;

        if (data < current->data)
            current = current->left;
        else if (data > current->data)
            current = current->right;
        else
        {
            printf("Duplicate value! Node not added.\n");
            return;
        }
    }

    newNode = createNode(data);
    newNode->parent = parent;

    if (parent == NULL)
        root = newNode;
    else if (data < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    rebalanceUpward(parent);

    printf("Node inserted successfully.\n");
}

/* SEARCH NODE ITERATIVELY */

struct TreeNode *findNode(struct TreeNode *node,
                          unsigned int data)
{
    while (node != NULL)
    {
        if (data == node->data)
            return node;

        if (data < node->data)
            node = node->left;
        else
            node = node->right;
    }

    return NULL;
}

/* FIND MINIMUM NODE */

struct TreeNode *findMinNode(struct TreeNode *node)
{
    while (node != NULL && node->left != NULL)
        node = node->left;

    return node;
}

/* REPLACE SUBTREE */

void transplant(struct TreeNode *oldNode,
                struct TreeNode *newNode)
{
    if (oldNode->parent == NULL)
        root = newNode;
    else if (oldNode->parent->left == oldNode)
        oldNode->parent->left = newNode;
    else
        oldNode->parent->right = newNode;

    if (newNode != NULL)
        newNode->parent = oldNode->parent;
}

/* DELETE NODE ITERATIVELY */

void deleteNode(unsigned int data)
{
    struct TreeNode *node;
    struct TreeNode *start;
    struct TreeNode *successor;

    node = findNode(root, data);

    if (node == NULL)
    {
        printf("Node not found!\n");
        return;
    }

    if (node->left == NULL)
    {
        start = node->parent;

        transplant(node, node->right);

        if (start == NULL)
            start = root;
    }
    else if (node->right == NULL)
    {
        start = node->parent;

        transplant(node, node->left);

        if (start == NULL)
            start = root;
    }
    else
    {
        successor = findMinNode(node->right);

        if (successor->parent != node)
        {
            start = successor->parent;

            transplant(successor, successor->right);

            successor->right = node->right;
            successor->right->parent = successor;
        }
        else
        {
            start = successor;
        }

        transplant(node, successor);

        successor->left = node->left;
        successor->left->parent = successor;

        updateHeight(successor);
    }

    free(node);

    if (start != NULL)
        rebalanceUpward(start);

    printf("Node deleted successfully.\n");
}

/* UPDATE NODE */

void updateNode(unsigned int oldData,
                unsigned int newData)
{
    if (findNode(root, oldData) == NULL)
    {
        printf("Old node not found!\n");
        return;
    }

    if (oldData == newData)
    {
        printf("Old and new values are the same.\n");
        return;
    }

    if (findNode(root, newData) != NULL)
    {
        printf("New value already exists!\n");
        return;
    }

    deleteNode(oldData);
    addNode(newData);

    printf("Node updated successfully.\n");
}

/* PREORDER TRAVERSAL */

void preorder(struct TreeNode *node)
{
    struct Stack s;
    struct TreeNode *current;

    initStack(&s);

    if (node == NULL)
        return;

    push(&s, node);

    while (!isStackEmpty(&s))
    {
        current = pop(&s);

        printf("%u ", current->data);

        if (current->right != NULL)
            push(&s, current->right);

        if (current->left != NULL)
            push(&s, current->left);
    }

    destroyStack(&s);
}

/* INORDER TRAVERSAL */

void inorder(struct TreeNode *node)
{
    struct Stack s;
    struct TreeNode *current;

    initStack(&s);
    current = node;

    while (current != NULL || !isStackEmpty(&s))
    {
        while (current != NULL)
        {
            push(&s, current);
            current = current->left;
        }

        current = pop(&s);

        printf("%u ", current->data);

        current = current->right;
    }

    destroyStack(&s);
}

/* POSTORDER TRAVERSAL */

void postorder(struct TreeNode *node)
{
    struct Stack s1;
    struct Stack s2;
    struct TreeNode *current;

    initStack(&s1);
    initStack(&s2);

    if (node == NULL)
        return;

    push(&s1, node);

    while (!isStackEmpty(&s1))
    {
        current = pop(&s1);
        push(&s2, current);

        if (current->left != NULL)
            push(&s1, current->left);

        if (current->right != NULL)
            push(&s1, current->right);
    }

    while (!isStackEmpty(&s2))
    {
        current = pop(&s2);
        printf("%u ", current->data);
    }

    destroyStack(&s1);
    destroyStack(&s2);
}

/* LEVEL ORDER TRAVERSAL */

void levelOrder(void)
{
    struct Queue q;
    struct TreeNode *current;

    if (root == NULL)
        return;

    q.front = NULL;
    q.rear = NULL;

    enqueue(&q, root);

    while (q.front != NULL)
    {
        current = dequeue(&q);

        printf("%u ", current->data);

        if (current->left != NULL)
            enqueue(&q, current->left);

        if (current->right != NULL)
            enqueue(&q, current->right);
    }
}

/* COUNT NODES ITERATIVELY */

size_t countNodes(struct TreeNode *node)
{
    struct Stack s;
    struct TreeNode *current;
    size_t count = 0;

    if (node == NULL)
        return 0;

    initStack(&s);
    push(&s, node);

    while (!isStackEmpty(&s))
    {
        current = pop(&s);
        count++;

        if (current->left != NULL)
            push(&s, current->left);

        if (current->right != NULL)
            push(&s, current->right);
    }

    destroyStack(&s);

    return count;
}

/* DISPLAY TREE */

void displayTree(void)
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

    printf("\nTotal Nodes : %zu\n", countNodes(root));
}

/* FREE TREE ITERATIVELY */

void freeTree(struct TreeNode *node)
{
    struct Stack s;
    struct TreeNode *current;

    if (node == NULL)
        return;

    initStack(&s);
    push(&s, node);

    while (!isStackEmpty(&s))
    {
        current = pop(&s);

        if (current->left != NULL)
            push(&s, current->left);

        if (current->right != NULL)
            push(&s, current->right);

        free(current);
    }

    destroyStack(&s);
}

/* MAIN FUNCTION */

int main(void)
{
    unsigned int choice;
    unsigned int data;
    unsigned int oldData;
    unsigned int newData;

    while (1)
    {
        printf("\n==============================\n");
        printf("          AVL TREE MENU\n");
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
                addNode(data);
                break;

            case 2:
                displayTree();
                break;

            case 3:
                data = getUnsignedInt("Enter data to search: ");

                if (findNode(root, data) != NULL)
                    printf("Node found!\n");
                else
                    printf("Node not found!\n");

                break;

            case 4:
                oldData = getUnsignedInt("Enter old data: ");
                newData = getUnsignedInt("Enter new data: ");

                updateNode(oldData, newData);
                break;

            case 5:
                data = getUnsignedInt("Enter data to delete: ");
                deleteNode(data);
                break;

            case 6:
                freeTree(root);
                root = NULL;

                printf("Program exited successfully.\n");
                return 0;

            default:
                printf("Invalid choice! Select 1-6.\n");
        }
    }
}
