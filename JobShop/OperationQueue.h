#pragma once
#include <random>
#include <algorithm>
#include "Operation.h"
#include "Operations.h"

using namespace std;

class OperationQueue
{
	protected:
		int getRandomIndex(int from, int to);
		void createOperationQueue(Operations operations);
	public:
		vector<vector<Operation>> _queue;
		OperationQueue(Operations operations);
		Operation popGreedyRandomPendingOperation();
		vector<Operation> getAllPendingOperations();
		bool isEmpty();
};

