#include "MazeGenerator.h"

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "utils/gl_utils.h"


struct bucket {
	int id;
	set<int> elements;
};

vector<bucket> buckets;    // #buckets = #cells = height * width
set<pair<int, int>> edges; // #edges = (height - 1) * width + (width - 1) * height
set<pair<int, int>> edgesConnected;
set<pair<int, int>> walls;


void MazeGenerator::PrintMaze() {

	for (auto it = walls.begin(); it != walls.end(); ++it)
		cout << (*it).first << " " << (*it).second << "\n";
}


void MazeGenerator::CreateSetAndBuckets(int height, int width) {
	int cell = 0;
	pair<int, int> newPair;
	bucket bkt;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (i != 0) {
				newPair.first = cell - width;
				newPair.second = cell;
				edges.insert(newPair);
			}
			else if (i != height - 1) {
				newPair.first = cell;
				newPair.second = cell + width;
				edges.insert(newPair);
			}
			else {
				newPair.first = cell - width;
				newPair.second = cell;
				edges.insert(newPair);
				newPair.first = cell;
				newPair.second = cell + width;
				edges.insert(newPair);
			}

			if (j != 0) {
				newPair.first = cell - 1;
				newPair.second = cell;
				edges.insert(newPair);
			}
			else if (j != width - 1) {
				newPair.first = cell;
				newPair.second = cell + 1;
				edges.insert(newPair);
			}
			else {
				newPair.first = cell - 1;
				newPair.second = cell;
				edges.insert(newPair);
				newPair.first = cell;
				newPair.second = cell + 1;
				edges.insert(newPair);
			}
			bkt.id = cell;
			cell++;
			buckets.push_back(bkt);
		}
	}
}

pair<int, int> MazeGenerator::CheckBucket(pair<int, int> edge) {
	int foundFirst = -1, foundSecond = -1;
	for (bucket bkt : buckets) {
		for (int element : bkt.elements) {
			if (element == edge.first) {
				foundFirst = bkt.id;
			}
			else if (element == edge.second) {
				foundSecond = bkt.id;
			}
		}
	}

	return make_pair(foundFirst, foundSecond);
}

void MazeGenerator::printBuckets() {
	for (bucket bkt : buckets) {
		for (int element : bkt.elements) {
			cout << element << " ";
		}
		cout << endl << endl;
	}
}

void MazeGenerator::MergeBuckets(pair<int, int>bucketCells, pair<int, int>edge) {
	vector<int> merged;
	int firstSize = 0;
	int secondSize = 0;

	if (bucketCells.first == -1 && bucketCells.second != -1) {
		buckets[bucketCells.second].elements.insert(edge.first);
		buckets[bucketCells.second].elements.insert(edge.second);
	}
	else if (bucketCells.second == -1 && bucketCells.first != -1) {
		buckets[bucketCells.first].elements.insert(edge.first);
		buckets[bucketCells.first].elements.insert(edge.second);
	}
	else if (bucketCells.first == -1 && bucketCells.second == -1) {
		for (bucket& bkt : buckets) {
			if (bkt.elements.size() == 0) {
				bkt.elements.insert(edge.first);
				bkt.elements.insert(edge.second);
				break;
			}
		}
	}
	else {
		for (int element : buckets[bucketCells.second].elements) {
			buckets[bucketCells.first].elements.insert(element);
		}
		buckets[bucketCells.second].elements.clear();
	}
}

set<pair<int, int>> MazeGenerator::GenerateMaze(int height, int width) {

	// Create a set with all connecting edges and a vector of buckets for each cell with their id
	CreateSetAndBuckets(height, width);
	walls = edges;

	// While the set of edges is not empty
	while (!edges.empty()) {
		// Randomly get an edge and remove it from the set
		auto it = edges.begin();
		int random = rand() % edges.size();
		std::advance(it, random);

		pair<int, int> edge = *it; // get

		// If cells are not already in the same bucket: Connect them and Merge Buckets
		pair<int, int> bucketCells = CheckBucket(edge);
		if (bucketCells.first != bucketCells.second || bucketCells.first < 0 || bucketCells.second < 0)
		{
			edgesConnected.insert(edge);
			MergeBuckets(bucketCells, edge);
		}

		edges.erase(it); // remove
	}

	for (pair<int, int> pair : edgesConnected) {
		walls.erase(pair);
	}

	return walls;
}
