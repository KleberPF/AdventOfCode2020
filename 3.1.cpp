#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
	std::ifstream input;
	input.open("input.txt");

	int numTrees = 0;
	int currentX = 3;

	std::string line;

	std::getline(input, line);
	while (std::getline(input, line))
	{
		if (line[currentX] == '#') { ++numTrees; }
		currentX += 3;
		currentX = currentX % line.length();
	}

	std::cout << "Number of trees: " << numTrees << '\n';

	input.close();
}
