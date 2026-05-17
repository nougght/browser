#include "HistoryRepository.h"
#include <iostream>

HistoryRepository::HistoryRepository(DatabaseManager *dbManager) : _dbManager(dbManager)
{}


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
