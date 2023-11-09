#include "Generation.h"

Schedule Generation::generateChild(Schedule parent1, Schedule parent2)
{
    vector<Schedule> parents = { parent1, parent2 };
    Schedule child_schedule(operations);
    int time_unit = 0;

    int min_makespan = min(parent1.makespan, parent2.makespan);

    while (!child_schedule.queue.isEmpty() && time_unit < min_makespan) {

        for (int machine = 0; machine < operations.num_machines; machine++) {
            
            if (shouldMutate()) {
                child_schedule.addRandomPendingOperation(machine, time_unit);
            }
            else {
                child_schedule.addOperationsByParents(parents, machine, time_unit);
            }
        }

        time_unit++;
    }

    child_schedule.createRandomSchedule();

    return child_schedule;
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
