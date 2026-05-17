#include "DatabaseManager.h"
#include <iostream>
#include <thread>

DatabaseManager::DatabaseManager(const std::string &dbPath) {
    sqlite3 *db;
    sqlite3_open(dbPath.c_str(), &db);

    _dbPtr.reset(db);

    std::string query = "CREATE TABLE IF NOT EXISTS history ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "url TEXT NOT NULL,"
                        "title TEXT,"
                        "visit_time INTEGER);";

    if (sqlite3_exec(_dbPtr.get(), query.c_str(), nullptr, nullptr, nullptr) ==
        0) {
        std::cout << "ok" << std::endl;
    }

    _dbThread = std::thread(([this] { run(); }));

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "end";
}

void DatabaseManager::run() {
    while (true) {
        std::function<void(sqlite3 *)> task;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _condVar.wait(lock, [this] { return !(this->_tasks.empty()); });
            task = std::move(_tasks.front());
            _tasks.pop();
        }
        task(_dbPtr.get());
    }
}

void DatabaseManager::post(std::function<void(sqlite3 *)> task) {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _tasks.push(task);
    }
    _condVar.notify_one();
}
