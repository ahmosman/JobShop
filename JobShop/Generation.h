#pragma once
#include "Schedule.h"
#include <algorithm>
#include <chrono>

using namespace std;

class Generation
{
	protected:
		Operations operations;

	public:
		float mutation_rate;
		int population, crossovers;
		Schedule generateChild(Schedule parent1, Schedule parent2);
		Generation(Operations operations, int population, int crossovers, float mutation_rate);
		bool shouldMutate();
		Schedule getOptimizedSchedule(int time_seconds);
		int getRandomIndex(int from, int to);
		vector<Schedule> getChildrenFromTournament(vector<Schedule> generation);
		void removeOperation(vector<Operation> &remove_from, Operation operation);
		bool checkSameOperations(Operation op1, Operation op2);
		void printCurrentMakespans(vector<Schedule> generation);
};

