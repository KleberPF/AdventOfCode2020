#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removeSpaces(char* s);
int numberOfCharStr(const char* s, char c);
unsigned long long int evaluateExpression(char* expression);
unsigned long long int evaluate(char* expression);
unsigned long long int part1(FILE* f);
unsigned long long int evaluate2(char* expression);
unsigned long long int evaluateExpression2(char* expression);
unsigned long long int part2(FILE* f);

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

//    unsigned long long int total = part1(f);
//    printf("Total = %llu\n", total);

    unsigned long long int total = part2(f);
    printf("Total = %llu\n", total);

    fclose(f);
    return 0;
}

void removeSpaces(char* s)
{
    char* d = s;
    do
    {
        while (*d == ' ')
        {
            ++d;
        }
    } while ((*s++ = *d++));
}

int numberOfCharStr(const char* s, char c)
{
    int count = 0;
    while (*s != 0)
    {
        if (*(s++) == c) { ++count; }
    }

    return count;
}

unsigned long long int evaluateExpression(char* expression)
{
    if (numberOfCharStr(expression, '(') != numberOfCharStr(expression, ')'))
    {
        printf("Invalid expression\n");
        return -1;
    }
    if (numberOfCharStr(expression, '(') == 0 && numberOfCharStr(expression, ')') == 0)
    {
        return evaluate(expression);
    }
    else
    {
        while (numberOfCharStr(expression, '(') != 0)
        {
            int openParCount = 0;
            int closeParCount = 0;
            int openParIndex = -1;
            int closeParIndex = -1;
            int length = (int)strlen(expression);
            for (int i = 0; i < length; ++i)
            {
                if (expression[i] == '(')
                {
                    ++openParCount;
                    openParIndex = i;
                }
                else if (expression[i] == ')')
                {
                    ++closeParCount;
                    if (closeParCount > openParCount)
                    {
                        printf("Invalid expression\n");
                        return -1;
                    }
                    else
                    {
                        closeParIndex = i;
                        break;
                    }
                }
            }
            char* exprStart = expression + openParIndex + 1; // + 1 to remove opening parenthesis
            char* exprEnd = expression + closeParIndex + 1;
            int charCount = closeParIndex - openParIndex - 1; // + 1 to remove closing parenthesis
            char expr[32] = "";
            strncat(expr, exprStart, charCount);
            unsigned long long int result = evaluate(expr);
            char resultStr[32];
            sprintf(resultStr, "%llu", result);
            char finalResult[1024] = "";
            strncat(finalResult, expression, openParIndex);
            strcat(finalResult, resultStr);
            strcat(finalResult, exprEnd);
            strcpy(expression, finalResult);
        }

        return evaluate(expression);
    }
    printf("Wrong\n");
}

unsigned long long int evaluate(char* expression)
{
    char* start = expression;
    unsigned long long int operand = 0;
    char operator = 0;
    int n = 0;
    unsigned long long int result;
    // find first expression
    sscanf(start, "%llu%n", &operand, &n);
    result = operand;
    start += n;
    while (sscanf(start, "%c%llu%n", &operator, &operand, &n) == 2)
    {
        if (operator == '+')
        {
            result += operand;
        }
        else if (operator == '*')
        {
            result *= operand;
        }
        start += n;
    }

    return result;
}

unsigned long long int part1(FILE* f)
{
    unsigned long long int total = 0;
    char line[1024];
    while (fgets(line, sizeof line, f))
    {
        // remove new line at the end
        line[strcspn(line, "\n")] = '\0';
        removeSpaces(line);
        total += evaluateExpression(line);
    }

    return total;
}

unsigned long long int evaluate2(char* expression)
{
    // split expression
    unsigned long long int result = 1;
    char expressions[32][512];
    int exprCount = 0;
    char* expr = strtok(expression, "*");
    if (expr != NULL)
    {
        while (expr != NULL)
        {
            strcpy(expressions[exprCount++], expr);
            expr = strtok(NULL, "*");
        }
    }
    else
    {
        // no multiplications
        strcpy(expressions[exprCount++], expression);
    }
    for (int i = 0; i < exprCount; ++i)
    {
        // calculate the sums
        unsigned long long int totalSum = 0;
        char* operand = strtok(expressions[i], "+");
        if (operand != NULL)
        {
            while (operand != NULL)
            {
                totalSum += strtoull(operand, NULL, 10);
                operand = strtok(NULL, "+");
            }
        }
        result *= totalSum;
    }

    return result;
}

unsigned long long int evaluateExpression2(char* expression)
{
    if (numberOfCharStr(expression, '(') != numberOfCharStr(expression, ')'))
    {
        printf("Invalid expression\n");
        return -1;
    }
    if (numberOfCharStr(expression, '(') == 0 && numberOfCharStr(expression, ')') == 0)
    {
        return evaluate2(expression);
    }
    else
    {
        while (numberOfCharStr(expression, '(') != 0)
        {
            int openParCount = 0;
            int closeParCount = 0;
            int openParIndex = -1;
            int closeParIndex = -1;
            int length = (int)strlen(expression);
            for (int i = 0; i < length; ++i)
            {
                if (expression[i] == '(')
                {
                    ++openParCount;
                    openParIndex = i;
                }
                else if (expression[i] == ')')
                {
                    ++closeParCount;
                    if (closeParCount > openParCount)
                    {
                        printf("Invalid expression\n");
                        return -1;
                    }
                    else
                    {
                        closeParIndex = i;
                        break;
                    }
                }
            }
            char* exprStart = expression + openParIndex + 1; // + 1 to remove opening parenthesis
            char* exprEnd = expression + closeParIndex + 1;
            int charCount = closeParIndex - openParIndex - 1; // + 1 to remove closing parenthesis
            char expr[32] = "";
            strncat(expr, exprStart, charCount);
            unsigned long long int result = evaluate2(expr);
            char resultStr[32];
            sprintf(resultStr, "%llu", result);
            char finalResult[1024] = "";
            strncat(finalResult, expression, openParIndex);
            strcat(finalResult, resultStr);
            strcat(finalResult, exprEnd);
            strcpy(expression, finalResult);
        }

        return evaluate2(expression);
    }
}

unsigned long long int part2(FILE* f)
{
    unsigned long long int ptotal = 0;
    unsigned long long int total = 0;
    char line[1024];
    while (fgets(line, sizeof line, f))
    {
        // remove new line at the end
        line[strcspn(line, "\n")] = '\0';
        removeSpaces(line);
        total += evaluateExpression2(line);
        if (total < ptotal)
        {
            printf("Overflow\n");
        }
        ptotal = total;
    }

    return total;
}
