#pragma once
#include <vector>
#include "Operations.h"
#include "Job.h"

using namespace std;

class Jobs
{
	public:
		vector<Job> jobs;
		void createJobs(Operations operations);
		Jobs(Operations operations);
};

