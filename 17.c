#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CYCLES 6

#define X_SIZE 32
#define Y_SIZE 32
#define Z_SIZE 32
#define W_SIZE 32

#define X_ORIGIN 12
#define Y_ORIGIN 12
#define Z_ORIGIN 12
#define W_ORIGIN 12

typedef struct cube
{
    int isActive;
    int willBeActive;
} Cube;

int fieldIndex3D(int x, int y, int z);
int numberOfActiveNeighbors3D(Cube* playingField3D, int x, int y, int z);
int fillPlayingField3D(FILE* f, Cube* playingField3D);
int calculate3D(Cube* playingField3D, int length);
int part1(FILE* f);

int fieldIndex4D(int x, int y, int z, int w);
int numberOfActiveNeighbors4D(Cube* playingField4D, int x, int y, int z, int w);
int fillPlayingField4D(FILE* f, Cube* playingField4D);
int calculate4D(Cube* playingField4D, int length);
int part2(FILE* f);

int main()
{
    FILE* f = fopen("../input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    int numberOfActiveCubes3D = part1(f);
    printf("Active cubes (3D): %d\n", numberOfActiveCubes3D);

    fseek(f, 0, SEEK_SET);

    int numberOfActiveCubes4D = part2(f);
    printf("Active cubes (4D): %d\n", numberOfActiveCubes4D);

    fclose(f);
    return 0;
}

int fieldIndex3D(int x, int y, int z)
{
    return x + y * X_SIZE + z * X_SIZE * Y_SIZE;
}

int numberOfActiveNeighbors3D(Cube* playingField3D, int x, int y, int z)
{
    int neighborsCount = 0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            for (int k = -1; k <= 1; ++k)
            {
                if (i != 0 || j != 0 || k != 0)
                {
                    if (playingField3D[fieldIndex3D(x + i, y + j, z + k)].isActive)
                    {
                        ++neighborsCount;
                    }
                }
            }
        }
    }

    return neighborsCount;
}

int fillPlayingField3D(FILE* f, Cube* playingField3D)
{
    int currentY = 0;
    char line[20];
    while(fscanf(f, "%s", line) == 1)
    {
        for (int x = 0; x < strlen(line); ++x)
        {
            if (line[x] == '.')
            {
                playingField3D[fieldIndex3D(X_ORIGIN + x, Y_ORIGIN + currentY, Z_ORIGIN)].isActive = 0;
            }
            else if (line[x] == '#')
            {
                playingField3D[fieldIndex3D(X_ORIGIN + x, Y_ORIGIN + currentY, Z_ORIGIN)].isActive = 1;
            }
            playingField3D[fieldIndex3D(X_ORIGIN + x, Y_ORIGIN + currentY, Z_ORIGIN)].willBeActive = 0;
        }
        ++currentY;
    }

    return currentY;
}

int calculate3D(Cube* playingField3D, int length)
{
    // current range to check
    int xRange[2] = {X_ORIGIN - 1, X_ORIGIN + length + 1};
    int yRange[2] = {Y_ORIGIN - 1, Y_ORIGIN + length + 1};
    int zRange[2] = {Z_ORIGIN - 1, Z_ORIGIN + 2};

    for (int i = 0; i < NUMBER_OF_CYCLES; ++i)
    {
        for (int z = zRange[0]; z < zRange[1]; ++z)
        {
            for (int y = yRange[0]; y < yRange[1]; ++y)
            {
                for (int x = xRange[0]; x < xRange[1]; ++x)
                {
                    int activeNeighborsCount = numberOfActiveNeighbors3D(playingField3D, x, y, z);

                    if (playingField3D[fieldIndex3D(x, y, z)].isActive)
                    {
                        if (activeNeighborsCount == 2 || activeNeighborsCount == 3)
                        {
                            playingField3D[fieldIndex3D(x, y, z)].willBeActive = 1;
                        }
                    }
                    else
                    {
                        if (activeNeighborsCount == 3)
                        {
                            playingField3D[fieldIndex3D(x, y, z)].willBeActive = 1;
                        }
                    }
                }
            }
        }

        // update cells
        for (int z = zRange[0]; z < zRange[1]; ++z)
        {
            for (int y = yRange[0]; y < yRange[1]; ++y)
            {
                for (int x = xRange[0]; x < xRange[1]; ++x)
                {
                    playingField3D[fieldIndex3D(x, y, z)].isActive = playingField3D[fieldIndex3D(x, y, z)].willBeActive;
                    playingField3D[fieldIndex3D(x, y, z)].willBeActive = 0;
                }
            }
        }

        // update ranges
        --xRange[0];
        ++xRange[1];
        --yRange[0];
        ++yRange[1];
        --zRange[0];
        ++zRange[1];
    }

    int numberOfActiveCubes = 0;
    for (int z = zRange[0]; z < zRange[1]; ++z)
    {
        for (int y = yRange[0]; y < yRange[1]; ++y)
        {
            for (int x = xRange[0]; x < xRange[1]; ++x)
            {
                if (playingField3D[fieldIndex3D(x, y, z)].isActive)
                {
                    ++numberOfActiveCubes;
                }
            }
        }
    }

    return numberOfActiveCubes;
}

int part1(FILE* f)
{
    // 3d array
    // element at index (x, y, z) = x + y * xSize + z * xSize * ySize
    Cube* playingField3D = malloc(sizeof *playingField3D * X_SIZE * Y_SIZE * Z_SIZE);
    memset(playingField3D, 0, sizeof *playingField3D * X_SIZE * Y_SIZE * Z_SIZE); // set playing field to 0

    // fill playing field
    int length = fillPlayingField3D(f, playingField3D);

    int numberOfActiveCubes = calculate3D(playingField3D, length);
    free(playingField3D);

    return numberOfActiveCubes;
}

int fieldIndex4D(int x, int y, int z, int w)
{
    return x + y * X_SIZE + z * X_SIZE * Y_SIZE + w * X_SIZE * Y_SIZE * Z_SIZE;
}

int numberOfActiveNeighbors4D(Cube* playingField4D, int x, int y, int z, int w)
{
    int neighborsCount = 0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            for (int k = -1; k <= 1; ++k)
            {
                for (int l = -1; l <= 1; ++l)
                {
                    if (i != 0 || j != 0 || k != 0 || l != 0)
                    {
                        if (playingField4D[fieldIndex4D(x + i, y + j, z + k, w + l)].isActive)
                        {
                            ++neighborsCount;
                        }
                    }
                }
            }
        }
    }

    return neighborsCount;
}

int fillPlayingField4D(FILE* f, Cube* playingField4D)
{
    int currentY = 0;
    char line[20];
    while(fscanf(f, "%s", line) == 1)
    {
        for (int x = 0; x < strlen(line); ++x)
        {
            if (line[x] == '.')
            {
                playingField4D[fieldIndex4D(X_ORIGIN + x, Y_ORIGIN + currentY, Z_ORIGIN, W_ORIGIN)].isActive = 0;
            }
            else if (line[x] == '#')
            {
                playingField4D[fieldIndex4D(X_ORIGIN + x, Y_ORIGIN + currentY, Z_ORIGIN, W_ORIGIN)].isActive = 1;
            }
            playingField4D[fieldIndex4D(X_ORIGIN + x, Y_ORIGIN + currentY, Z_ORIGIN, W_ORIGIN)].willBeActive = 0;
        }
        ++currentY;
    }

    return currentY;
}

int calculate4D(Cube* playingField4D, int length)
{
    // current range to check
    int xRange[2] = {X_ORIGIN - 1, X_ORIGIN + length + 1};
    int yRange[2] = {Y_ORIGIN - 1, Y_ORIGIN + length + 1};
    int zRange[2] = {Z_ORIGIN - 1, Z_ORIGIN + 2};
    int wRange[2] = {W_ORIGIN - 1, W_ORIGIN + 2};

    for (int i = 0; i < NUMBER_OF_CYCLES; ++i)
    {
        for (int w = wRange[0]; w < wRange[1]; ++w)
        {
            for (int z = zRange[0]; z < zRange[1]; ++z)
            {
                for (int y = yRange[0]; y < yRange[1]; ++y)
                {
                    for (int x = xRange[0]; x < xRange[1]; ++x)
                    {
                        int activeNeighborsCount = numberOfActiveNeighbors4D(playingField4D, x, y, z, w);

                        if (playingField4D[fieldIndex4D(x, y, z, w)].isActive)
                        {
                            if (activeNeighborsCount == 2 || activeNeighborsCount == 3)
                            {
                                playingField4D[fieldIndex4D(x, y, z, w)].willBeActive = 1;
                            }
                        }
                        else
                        {
                            if (activeNeighborsCount == 3)
                            {
                                playingField4D[fieldIndex4D(x, y, z, w)].willBeActive = 1;
                            }
                        }
                    }
                }
            }
        }


        // update cells
        for (int w = wRange[0]; w < wRange[1]; ++w)
        {
            for (int z = zRange[0]; z < zRange[1]; ++z)
            {
                for (int y = yRange[0]; y < yRange[1]; ++y)
                {
                    for (int x = xRange[0]; x < xRange[1]; ++x)
                    {
                        playingField4D[fieldIndex4D(x, y, z, w)].isActive = playingField4D[fieldIndex4D(x, y, z, w)].willBeActive;
                        playingField4D[fieldIndex4D(x, y, z, w)].willBeActive = 0;
                    }
                }
            }
        }

        // update ranges
        --xRange[0];
        ++xRange[1];
        --yRange[0];
        ++yRange[1];
        --zRange[0];
        ++zRange[1];
        --wRange[0];
        ++wRange[1];
    }

    int numberOfActiveCubes = 0;
    for (int w = wRange[0]; w < wRange[1]; ++w)
    {
        for (int z = zRange[0]; z < zRange[1]; ++z)
        {
            for (int y = yRange[0]; y < yRange[1]; ++y)
            {
                for (int x = xRange[0]; x < xRange[1]; ++x)
                {
                    if (playingField4D[fieldIndex4D(x, y, z, w)].isActive)
                    {
                        ++numberOfActiveCubes;
                    }
                }
            }
        }
    }

    return numberOfActiveCubes;
}

int part2(FILE* f)
{
    // 4d array
    // element at index (x, y, z) = x + y * xSize + z * xSize * ySize + w * xSize * ySize * zSize
    Cube* playingField4D = malloc(sizeof *playingField4D * X_SIZE * Y_SIZE * Z_SIZE * W_SIZE);
    memset(playingField4D, 0, sizeof *playingField4D * X_SIZE * Y_SIZE * Z_SIZE * W_SIZE); // set playing field to 0

    // fill playing field
    int length = fillPlayingField4D(f, playingField4D);

    int numberOfActiveCubes = calculate4D(playingField4D, length);
    free(playingField4D);

    return numberOfActiveCubes;
}
