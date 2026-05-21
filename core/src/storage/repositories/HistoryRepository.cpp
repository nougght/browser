#include "HistoryRepository.h"
#include <iostream>

HistoryRepository::HistoryRepository(DatabaseManager *dbManager,
                                     ICoreDispatcher *dispatcher)
    : _dbManager(dbManager), _dispatcher(dispatcher) {}

void HistoryRepository::addVisit(const HistoryEntry &entry) {
    _dbManager->post([entry](sqlite3 *db) {
        sqlite3_stmt *stmt = nullptr;

        sqlite3_prepare_v2(db, "INSERT INTO history(url, title) VALUES(?, ?)", -1,
                           &stmt, nullptr);

        sqlite3_bind_text(stmt, 1, entry.url.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, entry.title.c_str(), -1, SQLITE_STATIC);
        int rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE) {
            std::cout << "Success\n";
        } else {
            std::cout << "Error: " << sqlite3_errstr(rc) << "\n";
        }
        sqlite3_finalize(stmt);
    });
}

void HistoryRepository::getHistory(
    std::function<void(std::vector<HistoryEntry>)> callback) {
    _dbManager->post([this, callback](sqlite3 *db) {
        sqlite3_stmt *stmt = nullptr;

        sqlite3_prepare_v2(db, "SELECT * FROM history", -1, &stmt, nullptr);

        std::vector<HistoryEntry> result;
        std::cout << "vector size = " << result.size() << std::endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int64_t id = sqlite3_column_int64(stmt, 0);
            std::string url = (const char *)sqlite3_column_text(stmt, 1);
            std::string title = (const char *)sqlite3_column_text(stmt, 2);
            int64_t visit_time = sqlite3_column_int64(stmt, 3);
            result.push_back(HistoryEntry{id, url, title, visit_time});
            std::cout << url << "\n";
        }
        sqlite3_finalize(stmt);

        _dispatcher->post([callback = std::move(callback), result = std::move(result)]() {
            std::cout << "sqlite result vector size = " << result.size() << std::endl;
            callback(result);
        });
    });
}
