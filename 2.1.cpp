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

		int atLeast = std::stoi(std::string(line, i, j - i));
		++j;
		i = j;

		while (line[j] != ' ') { ++j; }

		int atMost = std::stoi(std::string(line, i, j - i));
		++j;
		i = j;

		char letter = line[i];
		i += 2;

		std::string password = std::string(line, i, line.length() - 1);

		int ocurrencies = 0;
		for (int k = 0; k < password.length(); ++k)
		{
			if (password[k] == letter) { ++ocurrencies; }
		}
		if (ocurrencies >= atLeast && ocurrencies <= atMost) { ++numValidPasswords; }
	}

	std::cout << "Valid passwords: " << numValidPasswords << '\n';

	input.close();
}
