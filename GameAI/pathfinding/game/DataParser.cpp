#include "DataParser.h"
#include <fstream>


int DataParser::ReadFile(std::string key)
{
	std::ifstream inputFile(mFileName);
	int intValue = 0;
	while (!inputFile.eof())
	{
		std::string data;
		std::getline(inputFile, data, ' ');
		//cout << data << "***************\n";
		if (data == key)
		{
			std::getline(inputFile, data, '(');
			std::getline(inputFile, data, ')');
			intValue = stoi(data);
		}
		else
		{
			getline(inputFile, data);
		}

	}
	return intValue;
	inputFile.close();
}

void DataParser::WriteToKey(int cohesion, int seperation, int alignment)
{
	std::ofstream outputFile(mFileName);
	outputFile << "Cohesion (" << cohesion << ")\nSeperation (" << seperation << ")\nAlignment (" << alignment << ")\n";
	outputFile.close();
}