#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define INPUT_SIZE 100

int heap[MAX];
int size = 0;
int isMaxHeap = 1;

// Function declarations
int readInput(char str[], int capacity);
int isNumber(const char str[]);
int readInteger(const char prompt[], int *value);
int readMenuChoice(const char prompt[], int min, int max, int *value);

void swap(int *a, int *b);
int hasPriority(int a, int b);
void heapifyUp(int index);
void heapifyDown(int index);

void createHeap(void);
void displayHeap(void);
void insertElement(void);
void updateElement(void);
void deleteElement(void);

// Read input using fgets()
int readInput(char str[], int capacity)
{
    if (fgets(str, capacity, stdin) == NULL)
        return 0;

    str[strcspn(str, "\n")] = '\0';

    return 1;
}

// Check whether input contains an integer
int isNumber(const char str[])
{
    int i = 0;

    if (str[i] == '+' || str[i] == '-')
        i++;

    if (str[i] == '\0')
        return 0;

    for (; str[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)str[i]))
            return 0;
    }

    return 1;
}

// Read an integer
int readInteger(const char prompt[], int *value)
{
    char input[INPUT_SIZE];

    while (1)
    {
        printf("%s", prompt);

        if (!readInput(input, sizeof(input)))
            return 0;

        if (isNumber(input))
        {
            *value = atoi(input);
            return 1;
        }

        printf("Invalid input! Enter numbers only.\n");
    }
}

// Validate menu choices
int readMenuChoice(const char prompt[], int min, int max, int *value)
{
    while (1)
    {
        if (!readInteger(prompt, value))
            return 0;

        if (*value >= min && *value <= max)
            return 1;

        printf("Invalid choice! Select from %d to %d.\n", min, max);
    }
}

// Swap two values
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Check priority for Max Heap or Min Heap
int hasPriority(int a, int b)
{
    if (isMaxHeap)
        return a > b;

    return a < b;
}

// Move element upwards
void heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (hasPriority(heap[index], heap[parent]))
        {
            swap(&heap[index], &heap[parent]);
            index = parent;
        }
        else
        {
            break;
        }
    }
}

// Move element downwards
void heapifyDown(int index)
{
    while (1)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int best = index;

        if (left < size &&
            hasPriority(heap[left], heap[best]))
        {
            best = left;
        }

        if (right < size &&
            hasPriority(heap[right], heap[best]))
        {
            best = right;
        }

        if (best != index)
        {
            swap(&heap[index], &heap[best]);
            index = best;
        }
        else
        {
            break;
        }
    }
}

// Create Heap
void createHeap(void)
{
    int n, i, value;
    int temp[MAX];

    readInteger("Enter number of elements (0-100): ", &n);

    if (n < 0 || n > MAX)
    {
        printf("Number of elements must be between 0 and 100.\n");
        return;
    }

    for (i = 0; i < n; i++)
    {
        printf("Enter element %d: ", i + 1);

        if (!readInteger("", &value))
            return;

        temp[i] = value;
    }

    for (i = 0; i < n; i++)
        heap[i] = temp[i];

    size = n;

    for (i = size / 2 - 1; i >= 0; i--)
        heapifyDown(i);

    printf("Heap created successfully!\n");
}

// Display Heap
void displayHeap(void)
{
    int i;

    if (size == 0)
    {
        printf("Heap is empty!\n");
        return;
    }

    printf("\nHeap type: %s\n",
           isMaxHeap ? "Max Heap" : "Min Heap");

    printf("Heap array: ");

    for (i = 0; i < size; i++)
        printf("%d ", heap[i]);

    printf("\nRoot element: %d\n", heap[0]);

    printf("Index: ");

    for (i = 0; i < size; i++)
        printf("%d ", i);

    printf("\n");
}

// Insert Element
void insertElement(void)
{
    int value;

    if (size == MAX)
    {
        printf("Heap is full!\n");
        return;
    }

    if (!readInteger("Enter value to insert: ", &value))
        return;

    heap[size] = value;
    size++;

    heapifyUp(size - 1);

    printf("Element inserted successfully!\n");
}

// Update Element
void updateElement(void)
{
    int index, value, oldValue;

    if (size == 0)
    {
        printf("Heap is empty!\n");
        return;
    }

    displayHeap();

    if (!readInteger("Enter index to update: ", &index))
        return;

    if (index < 0 || index >= size)
    {
        printf("Invalid index!\n");
        return;
    }

    if (!readInteger("Enter new value: ", &value))
        return;

    oldValue = heap[index];
    heap[index] = value;

    if (value != oldValue)
    {
        if (hasPriority(value, oldValue))
            heapifyUp(index);
        else
            heapifyDown(index);
    }

    printf("Element updated successfully!\n");
}

// Delete Element
void deleteElement(void)
{
    int index;

    if (size == 0)
    {
        printf("Heap is empty!\n");
        return;
    }

    displayHeap();

    if (!readInteger("Enter index to delete: ", &index))
        return;

    if (index < 0 || index >= size)
    {
        printf("Invalid index!\n");
        return;
    }

    printf("Deleted element: %d\n", heap[index]);

    heap[index] = heap[size - 1];
    size--;

    if (index < size)
    {
        if (index > 0 &&
            hasPriority(heap[index], heap[(index - 1) / 2]))
        {
            heapifyUp(index);
        }
        else
        {
            heapifyDown(index);
        }
    }

    printf("Element deleted successfully!\n");
}

// Main Menu
int main(void)
{
    int heapChoice, choice;

    while (1)
    {
        printf("\n========== HEAP TYPE MENU ==========\n");
        printf("1. Max Heap\n");
        printf("2. Min Heap\n");
        printf("3. Exit\n");

        if (!readMenuChoice("Enter choice: ", 1, 3, &heapChoice))
            break;

        if (heapChoice == 3)
        {
            printf("Exiting program...\n");
            break;
        }

        isMaxHeap = (heapChoice == 1);
        size = 0;

        while (1)
        {
            printf("\n========== %s MENU ==========\n",
                   isMaxHeap ? "MAX HEAP" : "MIN HEAP");

            printf("1. Create Heap\n");
            printf("2. Read / Display Heap\n");
            printf("3. Insert Element\n");
            printf("4. Update Element\n");
            printf("5. Delete Element\n");
            printf("6. Back to Heap Type Menu\n");

            if (!readMenuChoice("Enter choice: ", 1, 6, &choice))
                return 0;

            switch (choice)
            {
                case 1:
                    createHeap();
                    break;

                case 2:
                    displayHeap();
                    break;

                case 3:
                    insertElement();
                    break;

                case 4:
                    updateElement();
                    break;

                case 5:
                    deleteElement();
                    break;

                case 6:
                    goto heapTypeMenu;
            }
        }

        heapTypeMenu:
        ;
    }

    return 0;
}