#pragma once
#include "Trackable.h"
#include <string>

const std::string DEFAULT_FILE_NAME = "LoadData.txt";

class DataParser : public Trackable
{
public:
	static DataParser* getInstance();
	static void cleanUpInstance();
	static void initInstance(std::string fileName = DEFAULT_FILE_NAME);

	int ReadFile(std::string key);
	void WriteToKey(int cohesion, int seperation, int alignment);

private:
	static DataParser* mpsInstance;
	std::string mFileName;

	DataParser(std::string fileName) :mFileName(fileName) {}
	~DataParser() {}
};