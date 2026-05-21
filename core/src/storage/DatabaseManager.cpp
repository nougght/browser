#include "DatabaseManager.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>

DatabaseManager::DatabaseManager(const std::string &dbPath) {
    sqlite3 *db;
    sqlite3_open(dbPath.c_str(), &db);

    _dbPtr.reset(db);

    _dbThread = std::thread(([this, dbPath] { run(); }));

}

void DatabaseManager::run() {
    std::ifstream f("core_build/init.sql");
    if (f.is_open() == false){
        std::cout << "\nFILE NOT OPEN\n";
    } else {
        std::cout << "\nFILE OPEN\n";

    }
    std::stringstream ss;
    ss << f.rdbuf();
    std::cout << ss.str() << std::endl;
    if (sqlite3_exec(_dbPtr.get(), ss.str().c_str(), nullptr, nullptr, nullptr) ==
        0) {
        std::cout << "ok" << std::endl;
    }


    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "end";

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
