#include "task.h"
#include <chrono>

Task::Task(const std::string n, const std::string d,
           const std::chrono::time_point<std::chrono::steady_clock> start,
           const std::chrono::duration<double> len) {
    name = n;
    description = d;
    _start_point = start;
    _duration = len;
}

bool Task::isComplete() { return _completed; }

void Task::completeTask() { _completed = true; }

void Task::uncompleteTask() { _completed = false; }
