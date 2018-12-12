#include "DataParser.h"
#include <fstream>
#include <string>
#include <sstream>

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

void DataParser::WriteToKey(std::string key, int score)
{
	
	std::stringstream output;
	std::ifstream inputFile(mFileName);
	while (!inputFile.eof())
	{
		std::string data;
		std::getline(inputFile, data, ' ');

		output << data;

		if (data == key)
		{
			std::getline(inputFile, data, '(');
			output << " " << data;
			std::getline(inputFile, data, ')');
			//data.replace(data.find(data), data.length(), std::to_string(score));
			output << '(' << std::to_string(score) << ')';
		}
		else
		{
			getline(inputFile, data);
			output << ' ' << data;
		}
		output << "\n";

	}

	std::ofstream outputFile(mFileName);
	outputFile << output.str();
	outputFile.close();

	inputFile.close();
}