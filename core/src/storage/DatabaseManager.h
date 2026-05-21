#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <sqlite3.h>

class DatabaseManager {
private:
    void run();
    std::thread _dbThread;
    std::queue<std::function<void(sqlite3 *)>> _tasks;
    std::mutex _mutex;
    std::condition_variable _condVar;
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> _dbPtr{nullptr, sqlite3_close};

public:
    DatabaseManager(const std::string &dbPath);
    void post(std::function<void(sqlite3 *)>);
};

#endif
