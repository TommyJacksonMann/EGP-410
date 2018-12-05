#include "DataParser.h"
#include <fstream>

DataParser * DataParser::mpsInstance = nullptr;

DataParser* DataParser::getInstance()
{
	return mpsInstance;
}

void DataParser::cleanUpInstance()
{
	delete mpsInstance;
}

void DataParser::initInstance(std::string fileName)
{
	if (!mpsInstance)
	{
		mpsInstance = new DataParser(fileName);
	}
}

int DataParser::ReadFile(std::string key)
{
	std::ifstream inputFile(mFileName);
	int intValue = 0;
	while (!inputFile.eof())
	{
		std::string data;
		std::getline(inputFile, data, ' ');

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