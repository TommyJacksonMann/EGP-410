#pragma once
#include "Trackable.h"
#include <string>

class DataParser : public Trackable
{
public:
	DataParser(std::string fileName) :mFileName(fileName) {}
	~DataParser() {}

	int ReadFile(std::string key);
	void WriteToKey(int cohesion, int seperation, int alignment);

private:
	std::string mFileName;
};