#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int part1(FILE* f);
int part2(FILE* f);

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    int totalNumberOfQuestionsAnswered = part2(f);
    printf("%d", totalNumberOfQuestionsAnswered);

    fclose(f);
    return 0;
}

int part1(FILE* f)
{
    char questionsAnswered[26] = { 0 };
    int numberOfQuestionsAnswered = 0;
    int totalNumberOfQuestionsAnswered = 0;

    char line[40];
    while (fgets(line, sizeof line, f))
    {
        if (strcmp(line, "\n") == 0)
        {
            // new questionary
            totalNumberOfQuestionsAnswered += numberOfQuestionsAnswered;
            numberOfQuestionsAnswered = 0;
            continue;
        }
        else
        {
            line[strcspn(line, "\n")] = '\0'; // remove new line at the end
            for (int i = 0; i < strlen(line); ++i)
            {
                int letterFound = 0;
                for (int j = 0; j < numberOfQuestionsAnswered; ++j)
                {
                    if (questionsAnswered[j] == line[i])
                    {
                        letterFound = 1;
                        break;
                    }
                }
                if (!letterFound)
                {
                    questionsAnswered[numberOfQuestionsAnswered++] = line[i];
                }
            }
        }
    }
    // account for last questionary
    totalNumberOfQuestionsAnswered += numberOfQuestionsAnswered;

    return totalNumberOfQuestionsAnswered;
}

int part2(FILE* f)
{
    char* questions = NULL;
    int numberOfLines = 0;
    int validAnswers = 0;
    size_t questionsSize = 0;

    char line[40];
    char* result = line;
    while (result)
    {
        result = fgets(line, sizeof line, f);
        if (strcmp(line, "\n") == 0 || result == NULL)
        {
            // new questionary
            questions[strcspn(questions, "\n")] = '\0'; // remove new line at the end

            for (int i = 'a'; i <= 'z'; ++i)
            {
                int numberOfAnswers = 0;
                for (int j = 0; j < strlen(questions); ++j)
                {
                    if (i == questions[j]) { ++numberOfAnswers; }
                }
                if (numberOfAnswers == numberOfLines) { ++validAnswers; }
            }

            free(questions);
            questions = NULL;
            numberOfLines = 0;
            continue;
        }
        else
        {
            line[strcspn(line, "\n")] = '\0'; // remove new line at the end
            if (numberOfLines == 0)
            {
                questionsSize = strlen(line) + 1;
                questions = malloc(sizeof *questions * questionsSize);
                strcpy(questions, line);
            }
            else
            {
                questionsSize += strlen(line);
                questions = realloc(questions, sizeof *questions * questionsSize);
                strcat(questions, line);
            }

            ++numberOfLines;
        }
    }

    return validAnswers;
}
