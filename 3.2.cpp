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
	unsigned long treesProduct = 1;
	int right = 1;
	int down = 1;
	int currentX = right;
	std::vector<std::string> tobogganMap;

	std::string line;
	while (std::getline(input, line)) { tobogganMap.push_back(line); }

	for (int k = 0; k < 5; ++k)
	{
		for (int i = down; i < tobogganMap.size(); i += down)
		{
			if (tobogganMap[i][currentX] == '#') { ++numTrees; }
			currentX += right;
			currentX = currentX % tobogganMap[i].length();
		}

		treesProduct *= numTrees;
		right = right + 2;
		if (right > 7)
		{
			right = right % 8;
			++down;
		}
		currentX = right;
		numTrees = 0;
	}

	std::cout << "Total product: " << treesProduct << '\n';

	input.close();
}
