#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
	std::ifstream input;
	input.open("input.txt");

	int numValidPasswords = 0;

	std::string line;
	while (std::getline(input, line))
	{
		int i = 0;
		int j = 0;
		while (line[j] != '-') { ++j; }

		int firstIndex = std::stoi(std::string(line, i, j - i));
		++j;
		i = j;

		while (line[j] != ' ') { ++j; }

		int secondIndex = std::stoi(std::string(line, i, j - i));
		++j;
		i = j;

		char letter = line[i];
		i += 3;

		std::string password = std::string(line, i, line.length() - 1);

		bool validate = (password[firstIndex - 1] == letter && password[secondIndex - 1] != letter) ||
			(password[firstIndex - 1] != letter && password[secondIndex - 1] == letter);

		if (validate) { ++numValidPasswords; }
	}

	std::cout << "Valid passwords: " << numValidPasswords << '\n';

	input.close();
}
