#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STOP_NUMBER 30000000

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    char line[128];
    fgets(line, sizeof line, f);
    line[strcspn(line, "\n")] = '\0';
    int numbers[32];
    int numberCount = 0;
    char* num = strtok(line, ",");
    do
    {
        if (num != NULL)
        {
            numbers[numberCount++] = strtol(num, NULL, 10);
            num = strtok(NULL, ",");
        }
    } while (num != NULL);

    int* values = malloc(sizeof *values * 30000000);
    memset(values, 0, sizeof *values * 30000000);
    for (int i = 1; i <= numberCount; ++i)
    {
        values[numbers[i - 1]] = i;
    }
    int currentValue = numbers[numberCount - 1];

    for (int i = numberCount; i < STOP_NUMBER; ++i)
    {
        if (values[currentValue] == 0)
        {
            values[currentValue] = i;
            currentValue = 0;
        }
        else
        {
            int aux = currentValue;
            currentValue = i - values[currentValue];
            values[aux] = i;
        }
    }

    printf("%d\n", currentValue);

    free(values);

    fclose(f);
    return 0;
}
