#pragma once
#include "Trackable.h"
#include <string>
#include <fstream>

using namespace std;

const string FILE_SYSTEM_PATH = "FileSystem.txt";

class FileSystem : public Trackable
{
public:
	static FileSystem* getInstance();
	static void cleanUpInstance();
	static void initInstance();

	float loadInfoFloat(string keyword);
	void overWriteFloat(string keyword, float value);
private:
	static FileSystem* mpsInstance;
	FileSystem() {}
	~FileSystem() {}
};