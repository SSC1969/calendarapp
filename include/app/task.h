#pragma once
#include <chrono>
#include <string>

class Task {
  public:
    Task(const std::string n, const std::string d = "",
         const std::chrono::time_point<std::chrono::system_clock> start =
             std::chrono::system_clock::now(),
         const std::chrono::duration<double> len =
             std::chrono::duration<double>::zero());

    std::string name;
    std::string description;

    static void updateIdProvider(int new_provider) {
        Task::id_provider = new_provider;
    }

    int id() { return _id; }
    std::chrono::time_point<std::chrono::system_clock> start_point() {
        return _start_point;
    }
    std::chrono::duration<double> duration() { return _duration; }
    bool isComplete();
    void completeTask();
    void uncompleteTask();

  private:
    std::chrono::time_point<std::chrono::system_clock> _start_point;
    std::chrono::duration<double> _duration;
    bool _completed = false;

    int _id;
    static int id_provider;
};

// Add subtasks, repeating period, location, etc.
