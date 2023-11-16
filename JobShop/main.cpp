#include <iostream>
#include <fstream>
#include <vector>
#include "Schedule.h"
#include "Generation.h"

using namespace std;



int main() {

    int population = 50;
    int crossovers = 10;
    float mutation_rate = 0.4;

    Operations op("instance_fs2.txt");
    //Operations op("instance_test.txt");


    Generation gen(op, population, crossovers, mutation_rate);

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
    gen.optimize();
    return 0;
}