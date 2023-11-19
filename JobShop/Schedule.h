#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include "Operation.h"
#include "OperationQueue.h"

using namespace std;

class Schedule
{
	protected:
		vector<vector<Operation>> schedule;
		vector<Operation> recent_jobs_operations;
		Operation getRecentJobOperation(int job_no);
		void setRecentJobOperation(Operation operation);
		bool insertOperationInEmptySpace(Operation operation);
		void overrideEmptyOperation(int empty_operation_index, Operation operation);
		void insertOperationInEnd(Operation operation);

	public:
		int makespan, num_machines = 0, num_jobs = 0;
		OperationQueue queue;
		void createRandomSchedule();
		void createScheduleByJobsOrder(vector<int> job_order);
		Schedule(Operations operations);
		void addOperationToSchedule(Operation operation);
		vector<vector<Operation>> getScheduleQueue();
		void printSchedule();
		void printJobTimes();
		void printResult();
};

