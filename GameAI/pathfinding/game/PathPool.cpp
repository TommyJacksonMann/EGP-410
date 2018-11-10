#include "PathPool.h"
#include "Path.h"


PathPool::PathPool()
{
}


PathPool::~PathPool()
{

	for (auto iter = mPathPool.begin(); iter != mPathPool.end(); ++iter)
	{

	}
}

void PathPool::AddPath(FromID from, ToID to, Path* path)
{
	if (mPathPool.find(std::make_pair(from, to)) != mPathPool.end())
	{
		return;
	}

	mPathPool[std::make_pair(from, to)] = *path;
}

Path* PathPool::CheckPath(FromID from, ToID to)
{
	if (mPathPool.find(std::make_pair(from, to)) != mPathPool.end())
	{
		return &mPathPool[std::make_pair(from, to)];
	}
		return NULL;
	
/*
	for (auto iter = mPathPool.begin(); iter != mPathPool.end(); ++iter)
	{
		if (iter->first == std::make_pair(from, to))
		{
			return iter->second;
		}
	}
	return;
	*/
}