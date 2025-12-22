#include "app/database.h"
#include "app/constants.h"
#include <filesystem>
#include <format>
#include <sqlite3.h>
#include <string>
#include <wx/stdpaths.h>
#include <wx/wx.h>

CalenderDatabase::CalenderDatabase() {
    std::string data_path =
        wxStandardPaths::Get().GetUserDataDir().ToStdString();
    std::string db_path = data_path + "/" + constants::DB_NAME;

    if (!std::filesystem::exists(data_path)) {
        std::filesystem::create_directory(data_path);
    }

    // Set this class' database to be the calender database
    sqlite3_open(db_path.c_str(), &DB);

    if (!validateDatabaseFile()) {
        initializeDatabase();
    } else {
        Task::updateIdProvider(getTaskIdProvider());
    }
}
CalenderDatabase::~CalenderDatabase() {
    sqlite3_close(DB);
    DB = nullptr;
}

bool CalenderDatabase::validateDatabaseFile() {
    std::string sql =
        "SELECT 1 FROM sqlite_master WHERE type='table' AND NAME='tasks'";

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);

    // Should be a single row; the database is verified as having this
    // table
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        std::cout << "Database validated!\n";
        return true;
    }

    std::cout << "Database invalid!\n";
    return false;
}

void CalenderDatabase::initializeDatabase() {
    std::string sql = "CREATE TABLE 'tasks' ("
                      "'id'	INTEGER NOT NULL UNIQUE,"
                      "'name' TEXT NOT NULL,"
                      "'description' TEXT,"
                      "'start_point' TEXT NOT NULL,"
                      "'duration' TEXT,"
                      "'completed' INTEGER,"
                      "PRIMARY KEY('id'));";

    char *err;
    if (sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err) !=
        SQLITE_OK) {
        std::cout << "Error creating database: " << err << "\n";
        sqlite3_free(err);
    }
    Task::updateIdProvider(0);
}

int CalenderDatabase::getTaskIdProvider() {
    std::string sql = "SELECT MAX('id') FROM 'tasks'";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);
    int ec = sqlite3_step(stmt);
    int res = 0;
    if (ec == SQLITE_ROW) {
        res = sqlite3_column_int(stmt, 1);
    } else if (ec != SQLITE_DONE) {
        std::cout << "Error getting new ID provider! "
                  << sqlite3_errstr(ec) << "\n";
    }
    sqlite3_finalize(stmt);
    return res;
}

void CalenderDatabase::bindTaskParams(Task task, sqlite3_stmt *stmt) {
    sqlite3_bind_int(stmt, 1, task.id());
    sqlite3_bind_text(stmt, 2, task.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, task.description.c_str(), -1,
                      SQLITE_STATIC);

    std::string start_str = std::format("{:%F %R}", task.start_point());
    sqlite3_bind_text(stmt, 4, start_str.c_str(), -1, SQLITE_STATIC);
    std::string dur_str = std::format("{:%T}", task.duration());

    sqlite3_bind_text(stmt, 5, dur_str.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, task.isComplete());
}

// CRUD methods
void CalenderDatabase::addTask(Task task) {
    std::string sql = "INSERT INTO 'tasks' ('id', 'name', 'description', "
                      "'start_point', 'duration', 'completed')"
                      "VALUES(:1, :2, :3, :4, :5, :6)";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);
    bindTaskParams(task, stmt);

    int res = sqlite3_step(stmt);
    if (res != SQLITE_DONE) {
        std::cout << "Error adding task! " << sqlite3_errstr(res) << "\n";
    }
}
Task CalenderDatabase::updateTask(Task target_task, Task updated_task) {
    std::string sql = "UPDATE 'tasks'"
                      "WHERE 'name'=";
}
void CalenderDatabase::deleteTask(Task task) {}

std::vector<Task> CalenderDatabase::retrieveTasks(
    std::chrono::time_point<std::chrono::system_clock> date) {}
