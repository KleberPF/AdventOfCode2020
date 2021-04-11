#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct keyValuePair
{
    char key[4];
    char value[20];
} KeyValuePair;

typedef struct passport
{
    KeyValuePair fields[8];
    int numberOfFields;
} Passport;

int calculateValidPassportsPart2(FILE* f);
int isPassportValidPart2(Passport* passport);

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL) { printf("Fail opening file\n"); }
    int numberOfValidPassportsSecondCriteria = calculateValidPassportsPart2(f);
    
    printf("Number of valid passports (second criteria): %d", numberOfValidPassportsSecondCriteria);

    fclose(f);
    return 0;
}

int calculateValidPassportsPart2(FILE* f)
{
    int numberOfValidPassports = 0;

    // read passport data from input file
    char line[100];
    Passport passport;
    passport.numberOfFields = 0; // passport is empty

    while (fgets(line, sizeof line, f))
    {
        if (strcmp(line, "\n") == 0)
        {
            // new line means new passport
            // input validation
            if (isPassportValidPart2(&passport)) { ++numberOfValidPassports; }

            // clear passport
            for (int i = 0; i < passport.numberOfFields; ++i)
            {
                strcpy(passport.fields[i].key, "");
                strcpy(passport.fields[i].value, "");
            }
            passport.numberOfFields = 0;

            continue;
        }
        char* start = line;
        int offset;

        while (sscanf(start, " %[^:]:%s%n", passport.fields[passport.numberOfFields].key, passport.fields[passport.numberOfFields].value, &offset) == 2)
        {
            ++passport.numberOfFields;
            start += offset;
        }
    }
    // check last passport
    if (isPassportValidPart2(&passport)) { ++numberOfValidPassports; }


    return numberOfValidPassports;
}

int isPassportValidPart2(Passport* passport)
{
    // input validation
    const int smallestBirthYear = 1920;
    const int biggestBirthYear = 2002;
    const int smallestIssueYear = 2010;
    const int biggestIssueYear = 2020;
    const int smallestExpYear = 2020;
    const int biggestExpYear = 2030;
    const int smallestHeightCm = 150;
    const int biggestHeightCm = 193;
    const int smallestHeightIn = 59;
    const int biggestHeightIn = 76;
    // hair color - a # followed by exactly six characters 0-9 or a-f
    const char possibleEyeColors[7][4] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
    // passport ID - a nine-digit number, including leading zeroes
    // country ID - ignored, missing or not

    if (passport->numberOfFields < 7) { return 0; }
    else
    {
        if (passport->numberOfFields == 7)
        {
            // a passport with 7 fields can't have a cid
            for (int i = 0; i < 7; ++i)
            {
                if (strcmp(passport->fields[i].key, "cid") == 0) { return 0; }
            }
        }
        for (int i = 0; i < passport->numberOfFields; ++i)
        {
            if (strcmp(passport->fields[i].key, "byr") == 0)
            {
                if (strtol(passport->fields[i].value, NULL, 10) < smallestBirthYear || strtol(passport->fields[i].value, NULL, 10) > biggestBirthYear)
                {
                    return 0;
                }
            }
            else if (strcmp(passport->fields[i].key, "iyr") == 0)
            {
                if (strtol(passport->fields[i].value, NULL, 10) < smallestIssueYear || strtol(passport->fields[i].value, NULL, 10) > biggestIssueYear)
                {
                    return 0;
                }
            }
            else if (strcmp(passport->fields[i].key, "eyr") == 0)
            {
                if (strtol(passport->fields[i].value, NULL, 10) < smallestExpYear || strtol(passport->fields[i].value, NULL, 10) > biggestExpYear)
                {
                    return 0;
                }
            }
            else if (strcmp(passport->fields[i].key, "hgt") == 0)
            {
                char* unit;
                int height = strtol(passport->fields[i].value, &unit, 10);
                if (strcmp(unit, "cm") == 0)
                {
                    if (height < smallestHeightCm || height > biggestHeightCm)
                    {
                        return 0;
                    }
                }
                else if (strcmp(unit, "in") == 0)
                {
                    if (height < smallestHeightIn || height > biggestHeightIn)
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else if (strcmp(passport->fields[i].key, "hcl") == 0)
            {
                if (strlen(passport->fields[i].value) != 7)
                {
                    return 0;
                }
                else
                {
                    // check if first character is '#'
                    if (passport->fields[i].value[0] != '#')
                    {
                        return 0;
                    }
                    else
                    {
                        for (int j = 1; j < 7; ++j)
                        {
                            int isNumber = passport->fields[i].value[j] >= '0' && passport->fields[i].value[j] <= '9';
                            int isLetterAtoF = passport->fields[i].value[j] >= 'a' && passport->fields[i].value[j] <= 'f';
                            if (!(isNumber || isLetterAtoF))
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else if (strcmp(passport->fields[i].key, "ecl") == 0)
            {
                int colorFound = 0;
                for (int j = 0; j < 7; ++j) // 9 is the number of possible eye colors
                {
                    if (strcmp(passport->fields[i].value, possibleEyeColors[j]) == 0)
                    {
                        colorFound = 1;
                        break;
                    }
                }
                if (!colorFound)
                {
                    return 0;
                }
            }
            else if (strcmp(passport->fields[i].key, "pid") == 0)
            {
                if (strlen(passport->fields[i].value) != 9)
                {
                    return 0;
                }
                else
                {
                    for (int j = 0; j < 9; ++j)
                    {
                        if (passport->fields[i].value[j] < '0' || passport->fields[i].value[j] > '9')
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }

    return 1;
}



















