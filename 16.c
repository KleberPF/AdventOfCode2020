#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct field
{
    int leftRange[2];
    int rightRange[2];
} Field;

void readTicketLine(char* line, int* ticketNumbers, int* ticketCountPtr);
int isNumberValid(Field* fields, int fieldCount, int number);
int part1(FILE* f, Field* fields, int fieldCount);
int isTickedValid(Field* fields, int fieldCount, int* ticketNumbers, int ticketCount);
int isNumberInRange(Field* fields, int field, int number);
unsigned long long int  part2(FILE* f, Field* fields, int fieldCount);

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    Field fields[32];
    int fieldCount = 0;
    char line[200];
    // read fields
    while (strcmp(fgets(line, sizeof line, f), "\n") != 0)
    {
        sscanf(line, "%*[^:]: %d-%d %*s %d-%d", &fields[fieldCount].leftRange[0], &fields[fieldCount].leftRange[1],
               &fields[fieldCount].rightRange[0], &fields[fieldCount].rightRange[1]);
        ++fieldCount;
    }

//    int sumInvalidNumbers = part1(f, fields, fieldCount);
//    printf("Sum of invalid numbers: %d\n", sumInvalidNumbers);
    unsigned long long int result = part2(f, fields, fieldCount);
    printf("Result: %llu\n", result);

    fclose(f);
    return 0;
}

void readTicketLine(char* line, int* ticketNumbers, int* ticketCountPtr)
{
    char* numStr = strtok(line, ",");
    ticketNumbers[0] = strtol(numStr, NULL, 10);
    *ticketCountPtr = 1;

    int finished = 0;
    while (!finished)
    {
        numStr = strtok(NULL, ",");
        if (numStr != NULL)
        {
            ticketNumbers[*ticketCountPtr] = strtol(numStr, NULL, 10);
            ++(*ticketCountPtr);
        }
        else
        {
            finished = 1;
        }
    }
}

int isNumberValid(Field* fields, int fieldCount, int number)
{
    int valid = 0;

    for (int i = 0; i < fieldCount; ++i)
    {
        if ((number >= fields[i].leftRange[0] && number <= fields[i].leftRange[1]) || (number >= fields[i].rightRange[0] && number <= fields[i].rightRange[1]))
        {
            valid = 1;
            break;
        }
    }

    return valid;
}

int part1(FILE* f, Field* fields, int fieldCount)
{
    // read our ticket
    char line[200];
    fgets(line, sizeof line, f); // skip "your ticket:"
    int myTicketCount = 0;
    int myTicketNumbers[32];
    fgets(line, sizeof line, f);
    readTicketLine(line, myTicketNumbers, &myTicketCount);

    fgets(line, sizeof line, f); // skip new line
    fgets(line, sizeof line, f); // skip "nearby tickets:"
    // process all other tickets
    int sumInvalidNumbers = 0;
    int ticketCount = 0;
    int ticketNumbers[32];
    while (fgets(line, sizeof line, f))
    {
        readTicketLine(line, ticketNumbers, &ticketCount);
        // sum all invalid numbers
        for (int i = 0; i < ticketCount; ++i)
        {
            if (!isNumberValid(fields, fieldCount, ticketNumbers[i]))
            {
                sumInvalidNumbers += ticketNumbers[i];
            }
        }
    }

    return sumInvalidNumbers;
}

int isTickedValid(Field* fields, int fieldCount, int* ticketNumbers, int ticketCount)
{
    int valid = 1;
    for (int i = 0; i < ticketCount; ++i)
    {
        if (!isNumberValid(fields, fieldCount, ticketNumbers[i]))
        {
            valid = 0;
            break;
        }
    }

    return valid;
}

int isNumberInRange(Field* fields, int field, int number)
{
    return (number >= fields[field].leftRange[0] && number <= fields[field].leftRange[1]) ||
    (number >= fields[field].rightRange[0] && number <= fields[field].rightRange[1]);
}

unsigned long long int part2(FILE* f, Field* fields, int fieldCount)
{
    // read our ticket
    char line[200];
    fgets(line, sizeof line, f); // skip "your ticket:"
    int myTicketCount = 0;
    int myTicketNumbers[32];
    fgets(line, sizeof line, f);
    readTicketLine(line, myTicketNumbers, &myTicketCount);

    fgets(line, sizeof line, f); // skip new line
    fgets(line, sizeof line, f); // skip "nearby tickets:"
    // process all other tickets
    int ticketCount = 0;
    int ticketNumbers[32];
    int validTicketsCount = 0;
    int validTickets[512][32];

    while (fgets(line, sizeof line, f))
    {
        readTicketLine(line, ticketNumbers, &ticketCount);
        if (isTickedValid(fields, fieldCount, ticketNumbers, ticketCount))
        {
            for (int i = 0; i < ticketCount; ++i)
            {
                validTickets[validTicketsCount][i] = ticketNumbers[i];
            }
            ++validTicketsCount;
        }
    }

    int* fieldsPlacement = malloc(sizeof *fieldsPlacement * fieldCount);
    memset(fieldsPlacement, -1, sizeof *fieldsPlacement * fieldCount);

    int currentIndex = 0;
    while (currentIndex <= fieldCount)
    {
        int numberOfValidOptions = 0;
        int validField = 0;
        for (int j = 0; j < fieldCount; ++j)
        {
            if (fieldsPlacement[j] == -1)
            {
                int valid = 1;
                for (int k = 0; k < validTicketsCount; ++k)
                {
                    if (!isNumberInRange(fields, j, validTickets[k][currentIndex]))
                    {
                        valid = 0;
                        break;
                    }
                }
                if (valid)
                {
                    validField = j;
                    ++numberOfValidOptions;
                    if (numberOfValidOptions > 1)
                    {
                        break;
                    }
                }
            }
        }
        if (numberOfValidOptions == 1)
        {
            fieldsPlacement[validField] = currentIndex;
            currentIndex = 0;
        }
        else
        {
            ++currentIndex;
        }
    }

    unsigned long long int result = 1;
    for (int i = 0; i < 6; ++i)
    {
        result *= myTicketNumbers[fieldsPlacement[i]];
    }
    free(fieldsPlacement);

    return result;
}
