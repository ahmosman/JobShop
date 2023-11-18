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
		Operation getNextOperationForJob(int job_no);
		Operation popNextOperationForJob(int job_no);
		Operation popRandomPendingOperation();
		bool anyPendingOperationByMachine(int machine);
		Operation popRandomPendingOperationByMachine(int machine_number);
		bool isEmpty();
};

