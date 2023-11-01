#pragma once
#include <random>
#include "Operation.h"
#include "Operations.h"

using namespace std;

class OperationQueue
{
	vector<vector<Operation>> _queue;

	public:

		OperationQueue(Operations operations);
		int getRandomIndex(int from, int to);
		void createOperationQueue(Operations operations);
		Operation popRandomPendingOperation();
		bool isEmpty();
};

