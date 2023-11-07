#pragma once

#include <vector>
#include "Operation.h"
#include "OperationQueue.h"

using namespace std;

class Schedule
{
	protected:
		vector<int> current_job_time;
	public:
		vector<vector<Operation>> schedule;
		int makespan, num_machines = 0;
		OperationQueue queue;

		void createRandomSchedule();
		void createScheduleByJobsOrder(vector<int> job_order);
		Schedule(Operations operations);
		void addOperationInTimeUnit(Operation operation, int time_unit);
		void addOperationToSchedule(Operation operation);
		void addNewTimeUnitsToSchedule(int num_units);
		bool isMachineAvailable(int time_unit, Operation operation);
		bool isJobRunning(int time_unit, Operation operation);
		void printSchedule();
		int getCurrentTimeForJob(int job_no);
		void setCurrentTimeForJob(int job_no, int current_time);
};

