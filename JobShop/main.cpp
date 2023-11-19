#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include "Schedule.h"
#include "Generation.h"

using namespace std;



int main() {

    int population = 50;
    int crossovers = 10;
    float mutation_rate = 0.4;

   // Operations op("instance_fs2.txt");
    Operations op("instance_some.txt");


    Generation gen1(op, population, crossovers, mutation_rate);
    Operations op2 = op.getResizedOperations(5);
    Generation gen2(op2, population, crossovers, mutation_rate);

    /*

    Schedule sch1(op), sch2(op), sch3(op);

    sch1.createRandomSchedule();

    //sch1.createScheduleByJobsOrder({ 0, 0, 1, 0, 2, 0, 2, 2, 2, 1, 1, 1 });

    sch2.createRandomSchedule();


    sch1.printResult();
    sch2.printResult();

    Schedule child = gen.generateChild(sch1, sch2);


    child.printResult();

    */
    Schedule optimized2 = gen2.getOptimizedSchedule(5);
    optimized2.printJobTimes();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    //Schedule optimized2 = gen2.getOptimizedSchedule(10);
    //optimized2.printJobTimes();
 
    return 0;
}