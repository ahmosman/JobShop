#pragma once

#include <vector>
#include "Operation.h"
#include "OperationQueue.h"

using namespace std;

class Schedule
{
	protected:
		vector<vector<Operation>> _schedule;

	public:
		int makespan, num_machines = 0;
		OperationQueue queue;

		void createRandomSchedule();
		Schedule(Operations operations);
		void addOperationInTimeUnit(Operation operation, int time_unit);
		void addOperationToSchedule(Operation operation);
		void addNewTimeUnitsToSchedule(int num_units);
		void jobRunningInAnotherMachine(int time_unit, Operation operation);
		bool isMachineAvailable(int time_unit, int machine_num);
		bool isJobRunning(int time_unit, Operation operation);
		void printSchedule();
};

