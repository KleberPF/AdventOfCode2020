#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
	std::ifstream input;
	input.open("input.txt");

	std::vector<int> values;

	std::string line;
	while (std::getline(input, line))
	{
		values.push_back(std::stoi(line));
	}

	for (int i = 0; i < values.size(); ++i)
	{
		for (int j = i + 1; j < values.size(); ++j)
		{
			for (int k = j + 1; k < values.size(); ++k)
			{
				if (values[i] + values[j] + values[k] == 2020)
				{
					std::cout << "Multiplied: " << values[i] * values[j] * values[k] << '\n';
					goto end;
				}
			}
		}
	}
	end:

	input.close();
}
