#pragma once

#include <iostream>
#include <vector>
#include <set>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

using namespace std;

namespace MazeGenerator
{
	set<pair<int, int>> GenerateMaze(int height, int width);
	pair<int, int > CheckBucket(pair<int, int> edge);
	void CreateSetAndBuckets(int width, int height);
	void MergeBuckets(pair<int, int>bucketCells, pair<int, int>edge);
	void PrintMaze();
	void printBuckets();
}
