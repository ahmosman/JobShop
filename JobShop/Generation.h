#pragma once
#include "Schedule.h"
#include <algorithm>

using namespace std;

class Generation
{
	protected:
		Operations operations;

	public:
		float mutation_rate;
		int population;
		Schedule generateChild(Schedule parent1, Schedule parent2);
		Generation(Operations operations, int population, float mutation_rate);
		bool shouldMutate();
		int getRandomIndex(int from, int to);
		bool checkSameOperations(Operation op1, Operation op2);
		void addOperationsByParents(vector<Schedule> parents);
};

