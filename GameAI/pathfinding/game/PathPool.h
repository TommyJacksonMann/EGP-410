#pragma once
#include <map>
#include "Defines.h"
#include "Trackable.h"

class Path;

class PathPool : public Trackable
{
public:
	PathPool();
	~PathPool();

	void AddPath(FromID from, ToID to, Path* path);

	Path* CheckPath(FromID from, ToID to);

private:

	std::map<std::pair<FromID, ToID>, Path> mPathPool;
};

