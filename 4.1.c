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

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL) { printf("Fail opening file\n"); }

    Passport passports[1000];
    int currentPassport = 0;
    int currentField = 0;

    char key[4];
    char value[20];

    int c;
    while (!feof(f)) // evil, I know
    {
        c = getc(f);
        if (c == '\n')
        {
            ++currentPassport;
            currentField = 0;
            continue;
        }
        else
        {
            // read key
            key[0] = (char)c;
            key[1] = (char)getc(f);
            key[2] = (char)getc(f);
            key[3] = '\0'; // place null terminator at the end

            // discard ':'
            c = getc(f);

            // read value
            int index = 0;
            while (c != ' ' && c != '\n')
            {
                c = getc(f);
                if (c == EOF) { break; } // at the last line, instead of a new line we have EOF
                else if (c != ' ' && c != '\n') { value[index++] = (char)c; }
            }
            value[index] = '\0';  // place null terminator at the end

            strcpy(passports[currentPassport].fields[currentField].key, key);
            strcpy(passports[currentPassport].fields[currentField].value, value);
            ++currentField;

            ++passports[currentPassport].numberOfFields;
        }
    }

    int numberOfValidPassports = 0;
    for (int i = 0; i <= currentPassport; ++i)
    {
        if (passports[i].numberOfFields == 8) { ++numberOfValidPassports; }
        else if (passports[i].numberOfFields == 7)
        {
            int cidFound = 0;
            for (int j = 0; j < passports[i].numberOfFields; ++j)
            {
                if (strcmp(passports[i].fields[j].key, "cid") == 0)
                {
                    // passport contains cid and has 7 fields, therefore invalid
                    cidFound = 1;
                    break;
                }
            }
            if (!cidFound) { ++numberOfValidPassports; }
        }
    }

    printf("Number of valid passports: %d", numberOfValidPassports);

//    for (int i = 0; i <= currentPassport; ++i)
//    {
//        for (int j = 0; j < passports[i].numberOfFields; ++j)
//        {
//            printf("%s:%s\n", passports[i].fields[j].key, passports[i].fields[j].value);
//        }
//        printf("Number of fields: %d\n", passports[i].numberOfFields);
//        printf("---- New Passport ----\n");
//    }

    fclose(f);
    return 0;
}
