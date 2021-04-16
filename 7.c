#include <stdio.h>
#include <string.h>

typedef struct color
{
    char firstName[20];
    char secondName[20];
} Color;

typedef struct bag
{
    int index;
    Color color;
    int bagsInside[10][2];
    int bagsInsideCount;
} Bag;

int isBagInside(int start, Bag bags[20], int shinyGoldIndex);
int numberOfBagsInside(int start, Bag bags[20]);

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    Bag bags[1024];
    int bagCount = 0;
    int shinyGoldIndex;

    char line[200];
    while(fgets(line, sizeof line, f))
    {
        Bag bag;
        char* start = line;
        sscanf(start, "%s %s %*s %*s", bag.color.firstName, bag.color.secondName);
        bag.index = bagCount;
        if (strcmp(bag.color.firstName, "shiny") == 0 && strcmp(bag.color.secondName, "gold") == 0)
        {
            shinyGoldIndex = bag.index;
        }
        bag.bagsInsideCount = 0;
        bags[bagCount++] = bag;
    }
    fseek(f, 0, SEEK_SET); // go back to the start of the file

    int currentBag = 0;
    while(fgets(line, sizeof line, f))
    {
        Bag bag;
        char* start = line;
        int n;
        sscanf(start, "%*s %*s %*s %*s %n", &n); // ignore color of bag
        start += n;

        if (strcmp(start, "no other bags") != 0) // if bag has bags inside
        {
            int numberOfInsideBags;
            while (sscanf(start, "%d %s %s %*s %n", &numberOfInsideBags, bag.color.firstName, bag.color.secondName, &n) == 3)
            {
                // find bag index
                for (int i = 0; i < bagCount; ++i)
                {
                    if (strcmp(bag.color.firstName, bags[i].color.firstName) == 0 && strcmp(bag.color.secondName, bags[i].color.secondName) == 0)
                    {
                        bags[currentBag].bagsInside[bags[currentBag].bagsInsideCount][0] = i;
                        bags[currentBag].bagsInside[bags[currentBag].bagsInsideCount][1] = numberOfInsideBags;
                        ++bags[currentBag].bagsInsideCount;
                        break;
                    }
                }
                start += n;
            }

            ++currentBag;
        }
    }

    int validBags = 0;
    for (int i = 0; i < bagCount; ++i)
    {
        validBags += isBagInside(i, bags, shinyGoldIndex);
    }
    printf("Valid bags: %d\n", validBags);

    int amount = numberOfBagsInside(shinyGoldIndex, bags);
    printf("Bags inside gold bag: %d\n", amount);

    fclose(f);
    return 0;
}

int isBagInside(int start, Bag bags[20], int shinyGoldIndex)
{
    int found = 0;
    for (int i = 0; i < bags[start].bagsInsideCount; ++i)
    {
        if (bags[start].bagsInside[i][0] == shinyGoldIndex)
        {
            return 1;
        }
    }
    for (int i = 0; i < bags[start].bagsInsideCount; ++i)
    {
        found = isBagInside(bags[start].bagsInside[i][0], bags, shinyGoldIndex);
        if (found)
        {
            break;
        }
    }
    return found;
}

int numberOfBagsInside(int start, Bag bags[20])
{
    if (bags[start].bagsInsideCount == 0)
    {
        return 0;
    }
    else
    {
        int amount = 0;
        for (int i = 0; i < bags[start].bagsInsideCount; ++i)
        {
            amount += bags[start].bagsInside[i][1] + bags[start].bagsInside[i][1] * numberOfBagsInside(bags[start].bagsInside[i][0], bags);
        }
        return amount;
    }
}
