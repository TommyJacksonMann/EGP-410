#pragma once
#include "Trackable.h"
#include <string>

/*
* Created by RJ's for flocking assignment
* Tommy copied the file and implemneted it into this project
*
*/

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