#include "FileSystem.h"
#include <sstream>

FileSystem * FileSystem::mpsInstance = nullptr;

FileSystem* FileSystem::getInstance()
{
	return mpsInstance;
}

void FileSystem::cleanUpInstance()
{
	delete mpsInstance;
}

void FileSystem::initInstance()
{
	if (!mpsInstance)
	{
		mpsInstance = new FileSystem;
	}
}

float FileSystem::loadInfoFloat(string keyword)
{
	float floatValue = 0;
	ifstream inputFile(FILE_SYSTEM_PATH);
	while (!inputFile.eof())
	{
		string data;
		getline(inputFile, data, ' ');
		if (data == keyword)
		{
			getline(inputFile, data, '(');
			getline(inputFile, data, ')');
			floatValue = stof(data);
		}
		else
		{
			getline(inputFile, data);
		}
	}
	inputFile.close();
	return floatValue;
}

void FileSystem::overWriteFloat(string keyword, float value)
{
	float floatValue;
	ifstream inputFile(FILE_SYSTEM_PATH);
	string finalString = "";

	while (!inputFile.eof())
	{
		string data;
		getline(inputFile, data, ' ');
		
		finalString += data + " ";

		if (data == keyword)
		{
			getline(inputFile, data, '(');
			finalString += "(" + to_string(value) + ")\n";
			getline(inputFile, data);
		}
		else
		{
			getline(inputFile, data);
			finalString += data + "\n";
		}
	}
	inputFile.close();
	ofstream outputFile(FILE_SYSTEM_PATH);
	outputFile << finalString;
	outputFile.close();
}