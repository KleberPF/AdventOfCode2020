#include <stdio.h>
#include <stdlib.h>

#define PREAMBLE_SIZE 25

int compareInt (const void* a, const void* b);
unsigned long int part1(const unsigned long int numbers[1024], int numberCount);
unsigned long int part2(const unsigned long int numbers[1024], int numberCount, unsigned long int invalidNumber);

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    unsigned long int numbers[1024];
    int numberCount = 0;
    while (fscanf(f, "%lu", &numbers[numberCount]) == 1)
    {
        ++numberCount;
    }

    unsigned long int invalidNumber = part1(numbers, numberCount);
    unsigned long int minMaxSum = part2(numbers, numberCount, invalidNumber);

    printf("Invalid number: %lu\n", invalidNumber);
    printf("MinMaxSum: %lu", minMaxSum);

    fclose(f);
    return 0;
}

int compareInt (const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

unsigned long int part1(const unsigned long int numbers[1024], int numberCount)
{
    // find first invalid number
    for (int i = PREAMBLE_SIZE; i < numberCount; ++i)
    {
        int isNumberValid = 0;
        for (int j = 1; j <= PREAMBLE_SIZE; ++j)
        {
            int found = 0;
            for (int k = j; k <= PREAMBLE_SIZE; ++k)
            {
                if (numbers[i - j] + numbers[i - k] == numbers[i])
                {
                    isNumberValid = 1;
                    found = 1;
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }
        if (!isNumberValid)
        {
            return numbers[i];
        }
    }

    return -1;
}

unsigned long int part2(const unsigned long int numbers[1024], int numberCount, unsigned long int invalidNumber)
{
    for (int i = 0; i < numberCount - 2; ++i)
    {
        int found = 0;
        int numbersFoundCount = 1;
        unsigned long int sum = numbers[i];
        for (int j = i + 1; sum < invalidNumber; ++j)
        {
            ++numbersFoundCount;
            sum += numbers[j];
            if (sum == invalidNumber)
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            unsigned long int* arr = malloc(sizeof *arr * numbersFoundCount);
            for (int k = 0; k < numbersFoundCount; ++k)
            {
                arr[k] = numbers[i + k];
            }
            qsort(arr, numbersFoundCount, sizeof *arr, compareInt);
            unsigned long int minMaxSum = arr[0] + arr[numbersFoundCount - 1];
            free(arr);
            return minMaxSum;
        }
    }

    return -1;
}
