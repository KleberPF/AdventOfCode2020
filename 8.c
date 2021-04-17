#include <stdio.h>
#include <string.h>

typedef struct instruction
{
    char name[4];
    int operand;
    int hasBeenExecuted;
    int hasBeenChecked;
} Instruction;

int part1(Instruction instructions[1024], int instructionCount);
int part2(Instruction instructions[1024], int instructionCount);

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    Instruction instructions[1024];
    int instructionsCount = 0;

    char instructionName[4];
    int operand;
    while (fscanf(f, "%s %d", instructionName, &operand) == 2)
    {
        Instruction instruction;
        strcpy(instruction.name, instructionName);
        instruction.operand = operand;
        instruction.hasBeenExecuted = 0;
        instruction.hasBeenChecked = 0;
        instructions[instructionsCount++] = instruction;
    }

    int accumulator = part2(instructions, instructionsCount);
    
    printf("Accumulator: %d", accumulator);

    fclose(f);
    return 0;
}

int part1(Instruction instructions[1024], int instructionCount)
{
    int accumulator = 0;
    int currentInstruction = 0;

    while (currentInstruction < instructionCount)
    {
        if (instructions[currentInstruction].hasBeenExecuted == 1)
        {
            break;
        }
        else
        {
            instructions[currentInstruction].hasBeenExecuted = 1;
            if (strcmp(instructions[currentInstruction].name, "acc") == 0)
            {
                accumulator += instructions[currentInstruction].operand;
                ++currentInstruction;
            }
            else if (strcmp(instructions[currentInstruction].name, "jmp") == 0)
            {
                currentInstruction += instructions[currentInstruction].operand;
            }
            else
            {
                ++currentInstruction;
            }
        }
    }

    return accumulator;
}

int part2(Instruction instructions[1024], int instructionCount)
{
    int accumulator = 0;
    int savedAccumulator = 0;
    int currentInstruction = 0;
    int changedInstruction = 0;
    int isPathBeingChecked = 0;
    while (currentInstruction < instructionCount)
    {
        while (instructions[currentInstruction].hasBeenExecuted != 1)
        {
            instructions[currentInstruction].hasBeenExecuted = 1;
            if (strcmp(instructions[currentInstruction].name, "acc") == 0)
            {
                accumulator += instructions[currentInstruction].operand;
                ++currentInstruction;
            }
            else if (!isPathBeingChecked && instructions[currentInstruction].hasBeenChecked == 0)
            {
                // this is the instruction to be changed
                // jmp -> nop and nop -> jmp
                isPathBeingChecked = 1;
                savedAccumulator = accumulator;
                changedInstruction = currentInstruction;
                instructions[currentInstruction].hasBeenChecked = 1;
                if (strcmp(instructions[currentInstruction].name, "jmp") == 0)
                {
                    // jmp is now a nop
                    ++currentInstruction;
                }
                else
                {
                    // nop is now a jump
                    currentInstruction += instructions[currentInstruction].operand;
                }
            }
            else
            {
                // execute normally
                if (strcmp(instructions[currentInstruction].name, "jmp") == 0)
                {
                    currentInstruction += instructions[currentInstruction].operand;
                }
                else
                {
                    ++currentInstruction;
                }
            }
            if (currentInstruction == instructionCount)
            {
                // program reached the end
                return accumulator;
            }
        }
        isPathBeingChecked = 0;
        currentInstruction = changedInstruction;
        accumulator = savedAccumulator;
        for (int i = 0; i < instructionCount; ++i)
        {
            instructions[i].hasBeenExecuted = 0;
        }
    }

    return accumulator;
}
