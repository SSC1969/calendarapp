#pragma once
#include "app/task.h"
#include <memory>
#include <sqlite_orm/sqlite_orm.h>
#include <string>

// Define the relation between the Task class and the database
inline auto initStorage(const std::string &path) {
    using namespace sqlite_orm;
    return make_storage(
        path,
        make_table("tasks",
                   make_column("id", &Task::setId, &Task::getId,
                               primary_key().autoincrement()),
                   make_column("name", &Task::name),
                   make_column("description", &Task::description),
                   make_column("start", &Task::getStart, &Task::setStart),
                   make_column("end", &Task::getEnd, &Task::setEnd),
                   make_column("completed", &Task::getCompleted,
                               &Task::setCompleted)));
}

using Storage = decltype(initStorage(""));

class CalenderDatabase {
  public:
    CalenderDatabase();
    std::unique_ptr<Storage> storage;
};
