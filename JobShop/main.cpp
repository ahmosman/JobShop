#include <iostream>
#include <fstream>
#include <vector>
#include "Schedule.h"

using namespace std;



int main() {

    Operations op("instance_test.txt");

    Schedule sch1(op);

    sch1.createRandomSchedule();

    sch1.printSchedule();

    return 0;
}