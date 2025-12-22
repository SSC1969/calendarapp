#pragma once
#include "app/task.h"
#include <chrono>
#include <sqlite3.h>
#include <vector>

class CalenderDatabase {
  public:
    // Constructor/Destructor
    CalenderDatabase();
    ~CalenderDatabase();

    // CRUD methods
    void addTask(Task task);
    Task updateTask(Task target_task, Task updated_task);
    void deleteTask(Task task);

    std::vector<Task>
    retrieveTasks(std::chrono::time_point<std::chrono::system_clock> date);

  private:
    sqlite3 *DB;

    // Setup methods
    bool validateDatabaseFile();
    void initializeDatabase();
    int getTaskIdProvider();

    // Helper functions
    void bindTaskParams(Task task, sqlite3_stmt *stmt);
};
