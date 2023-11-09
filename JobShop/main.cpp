#include <iostream>
#include <fstream>
#include <vector>
#include "Schedule.h"
#include "Generation.h"

using namespace std;



int main() {

    int population = 10;
    float mutation_rate = 0.7;

    Operations op("instance_ft06.txt");

    Schedule sch1(op), sch2(op), sch3(op);


    Generation gen(op, population, mutation_rate);

    sch1.createRandomSchedule();

    //sch1.createScheduleByJobsOrder({ 1, 2, 1, 2, 0, 2, 2, 1, 0, 1, 0, 0 });

    sch2.createRandomSchedule();


    sch1.printSchedule();
    sch2.printSchedule();

    Schedule child = gen.generateChild(sch1, sch2);


    child.printSchedule();
    return 0;
}