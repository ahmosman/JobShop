#pragma once
#include "Schedule.h"
#include <chrono>

class Grasp
{
	protected:
		Operations operations;
	public:
		
	Grasp(Operations operations);
	Schedule getOptimizedSchedule(int time_seconds);
};

