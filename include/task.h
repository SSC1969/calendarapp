#pragma once
#include <chrono>
#include <string>

class Task {
  public:
    Task(const std::string n, const std::string d = "",
         const std::chrono::time_point<std::chrono::steady_clock> start =
             std::chrono::steady_clock::now(),
         const std::chrono::duration<double> len =
             std::chrono::duration<double>::zero());

    std::string name;
    std::string description;

    std::chrono::time_point<std::chrono::steady_clock> start_point() {
        return _start_point;
    }
    std::chrono::duration<double> duration() { return _duration; }
    bool isComplete();
    void completeTask();
    void uncompleteTask();

  private:
    std::chrono::time_point<std::chrono::steady_clock> _start_point;
    std::chrono::duration<double> _duration;
    bool _completed = false;
};

// Add subtasks, repeating period, location, etc.
