#include <stdio.h>
#include <stdlib.h>

int decodeSeatPosition(const char* seatString, int airplaneSeats[128][8]);

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    int airplaneSeats[128][8] = { 0 };
    int biggestSeatID = 0;

    char line[20];
    while(fscanf(f, "%s", line) == 1)
    {
        int seatID = decodeSeatPosition(line, airplaneSeats);
        if (seatID > biggestSeatID) { biggestSeatID = seatID; }
    }
    printf("Biggest seat ID: %d\n", biggestSeatID);

    // find empty seat
    int seatRow = 0;
    int seatColumn = 0;
    for (int i = 0; i < 128; ++i)
    {
        for (int j = 1; j < 8; ++j)
        {
            if (airplaneSeats[i][j - 1] == 1 && airplaneSeats[i][j] == 0 && airplaneSeats[i][j + 1] == 1)
            {
                seatRow = i;
                seatColumn = j;
                goto foundSeat;
            }
        }
    }
    foundSeat:

    printf("Your seat ID: %d", seatRow * 8 + seatColumn);

    fclose(f);
    return 0;
}

int decodeSeatPosition(const char* seatString, int airplaneSeats[128][8])
{
    int rowF = 127;
    int rowB = 0;
    int columnL = 7;
    int columnR = 0;
    int seatID;

    // parse first 7 characters to determine row
    for (int i = 0; i < 7; ++i)
    {
        int rowAverage = (rowF + rowB) / 2;
        if (seatString[i] == 'F') { rowF = rowAverage; }
        else if (seatString[i] == 'B') { rowB = rowAverage + 1; }
    }
    // parse last 3 characters to determine column
    for (int i = 7; i < 10; ++i)
    {
        int columnAverage = (columnR + columnL) / 2;
        if (seatString[i] == 'L') { columnL = columnAverage; }
        else if (seatString[i] == 'R') { columnR = columnAverage + 1; }
    }
    airplaneSeats[rowF][columnL] = 1;
    seatID = rowF * 8 + columnL;

    return seatID;
}
