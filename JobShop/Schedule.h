#pragma once

#include <vector>
#include <random>
#include "Operation.h"
#include "OperationQueue.h"

using namespace std;

class Schedule
{
	protected:
		vector<int> current_job_time;
		void addOperationInTimeUnit(Operation operation, int time_unit);
		void addNewTimeUnitsToSchedule(int num_units);

	public:
		vector<vector<Operation>> schedule;
		vector<vector<bool>> performed_operations;
		int makespan, num_machines = 0, num_jobs = 0;
		OperationQueue queue;

		void createRandomSchedule();
		void createScheduleByJobsOrder(vector<int> job_order);
		Schedule(Operations operations);
		void addOperationToSchedule(Operation operation, int time_unit = 0);
		bool isMachineAvailable(int time_unit, Operation operation);
		bool isJobRunning(int time_unit, Operation operation);
		void printSchedule();
		int getCurrentTimeForJob(int job_no);
		void setCurrentTimeForJob(int job_no, int current_time);
		bool isOperationPerformed(Operation operation);
		int getRandomIndex(int from, int to);
		void addOperationsByParents(vector<Schedule> parents, int machine, int time_unit);
		bool checkSameOperations(Operation op1, Operation op2);
		void addRandomPendingOperation(int machine, int time_unit);
		void setPerformedOperation(Operation operation);
};

