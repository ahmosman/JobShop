#include "Generation.h"

Schedule Generation::generateChild(Schedule parent1, Schedule parent2)
{
    vector<Schedule> parents = { parent1, parent2 };
    Schedule childSchedule(operations);
    int time_unit = 0;

    while (!childSchedule.queue.isEmpty()) {

        for (int machine = 0; machine < operations.num_machines; machine++) {
            
            int index = getRandomIndex(0, 2);

            Operation parent_inherit_operation = parents[index].schedule[machine][time_unit];

        }

        time_unit++;
    }

    return childSchedule;
}

Generation::Generation(Operations operations, int population, float mutation_rate): operations(operations)
{
    this->population = population;
    this->mutation_rate = mutation_rate;
}

bool Generation::shouldMutate()
{

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0, 1.0);

    float random_value = dis(gen);

    return random_value < mutation_rate;
}


int Generation::getRandomIndex(int from, int to)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(from, to - 1);

    return dis(gen);
}