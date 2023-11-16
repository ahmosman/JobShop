#pragma once
#include <random>
#include <algorithm>
#include "Operation.h"
#include "Operations.h"

using namespace std;

class OperationQueue
{
	public:
		vector<vector<Operation>> _queue;
		OperationQueue(Operations operations);
		int getRandomIndex(int from, int to);
		Operation getNextOperationForJob(int job_no);
		Operation popNextOperationForJob(int job_no);
		void createOperationQueue(Operations operations);
		Operation popRandomPendingOperation();
		bool anyPendingOperationByMachine(int machine);
		Operation popRandomPendingOperationByMachine(int machine_number);
		bool isEmpty();
};

