#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Operation.h"

using namespace std;

class Operations
{
    protected:
        string filename;
        vector<vector<int>> operations_duration;
        vector<vector<int>> operations_machines;


        vector<vector<Operation>> getReversedOperations(vector<vector<Operation>> operations);

    public:
        vector<vector<Operation>> _operations;
        vector<vector<Operation>> _reversed_operations;
        int num_jobs, num_machines;
        Operations(string filename);
        Operations(const Operations& operations);
        Operations getResizedOperations(int limit);
        int getJobsNum();
        void readInstanceTaillard();
        void readInstanceBeasley();
};
