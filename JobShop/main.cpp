#include <iostream>
#include <fstream>
#include <vector>
#include "Schedule.h"

using namespace std;



int main() {

    Operations op("instance_test.txt");

    Schedule sch1(op), sch2(op), sch3(op);

    sch1.createRandomSchedule();

    //sch1.createScheduleByJobsOrder({ 1, 2, 1, 2, 0, 2, 2, 1, 0, 1, 0, 0 });

    sch2.createRandomSchedule();
    sch3.createRandomSchedule();

    sch1.printSchedule();
    sch2.printSchedule();
    sch3.printSchedule();
    return 0;
}