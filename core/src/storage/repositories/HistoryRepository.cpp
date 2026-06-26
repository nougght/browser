#include "HistoryRepository.h"
#include "domain/InternalErrors.h"
#include <iostream>

HistoryRepository::HistoryRepository(DatabaseManager *dbManager,
                                     ICoreDispatcher *dispatcher)
    : _dbManager(dbManager), _dispatcher(dispatcher) {}

void HistoryRepository::addVisit(HistoryEntry &entry,
                                 HistoryAddedCallback callback) {
    _dbManager->post([this, entry = std::move(entry),
                      callback = std::move(callback)](sqlite3 *db) mutable {
        sqlite3_stmt *stmt = nullptr;

        sqlite3_prepare_v2(db, "INSERT INTO history(url, title) VALUES(?, ?)"
            "RETURNING id", -1,
                           &stmt, nullptr);

        sqlite3_bind_text(stmt, 1, entry.url.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, entry.title.c_str(), -1, SQLITE_STATIC);
        int rc = sqlite3_step(stmt);

        RepositoryError error;
        if (rc == SQLITE_ROW) {
            entry.id = sqlite3_column_int(stmt, 0);
            error = RepositoryError{RepositoryErrorCode::Success, ""};
        } else {
            error = RepositoryError{RepositoryErrorCode::DatabaseError,
                                    sqlite3_errstr(rc)};
        }
        _dispatcher->post([callback = std::move(callback), entry = std::move(entry),
                           error = std::move(error)] {
            callback(error.code == RepositoryErrorCode::Success
                         ? std::make_optional(std::move(entry))
                         : std::nullopt,
                     error);
        });
        sqlite3_finalize(stmt);
    });
}

void HistoryRepository::updateEntry(HistoryEntry &entry, HistoryUpdateCallback callback) {
    _dbManager->post([this, entry = std::move(entry), callback = std::move(callback)](sqlite3 *db) {
        sqlite3_stmt *stmt = nullptr;

        sqlite3_prepare_v2(db, "UPDATE history SET url = ?, title = ?"
                               "WHERE id = ?", -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, entry.url.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, entry.title.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 3, entry.id);
        auto rc = sqlite3_step(stmt);
        
        RepositoryError error;
        if (rc == SQLITE_DONE) {
            error = RepositoryError{RepositoryErrorCode::Success, ""};
        } else if (rc == SQLITE_NOTFOUND) {
            error = RepositoryError{RepositoryErrorCode::NotFound, "Entry not found"};
        } else {
            error = RepositoryError{RepositoryErrorCode::DatabaseError, sqlite3_errstr(rc)};
        }

        _dispatcher->post([callback = std::move(callback), error = std::move(error)]{
            callback(error);
        });
    });
}

void HistoryRepository::getHistory(HistoryGetCallback callback) {
    _dbManager->post([this, callback = std::move(callback)](sqlite3 *db) {
        sqlite3_stmt *stmt = nullptr;

        sqlite3_prepare_v2(db, "SELECT * FROM history", -1, &stmt, nullptr);

        std::vector<HistoryEntry> result;
        std::cerr << "vector size = " << result.size() << std::endl;
        int rc;
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int64_t id = sqlite3_column_int64(stmt, 0);
            std::string url = (const char *)sqlite3_column_text(stmt, 1);
            std::string title = (const char *)sqlite3_column_text(stmt, 2);
            int64_t visit_time = sqlite3_column_int64(stmt, 3);
            result.push_back(HistoryEntry{id, url, title, visit_time});
            std::cerr << url << "\n";
        }
        sqlite3_finalize(stmt);
        RepositoryError error;
        if (rc != SQLITE_DONE) {
            error = RepositoryError{RepositoryErrorCode::DatabaseError,
                                    sqlite3_errstr(rc)};
        } else {
            error = RepositoryError{RepositoryErrorCode::Success, ""};
        }
        _dispatcher->post([callback = std::move(callback),
                           result = std::move(result), error = std::move(error)]() {
            std::cerr << "sqlite result vector size = " << result.size() << std::endl;
            callback(std::move(result), std::move(error));
        });
    });
}

void HistoryRepository::deleteEntry(int64_t id, HistoryDeletedCallback callback) {
    _dbManager->post([this, id = std::move(id), callback = std::move(callback)](sqlite3 *db) {
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare_v2(db, "DELETE FROM history WHERE id = ?", -1, &stmt,
                           nullptr);

        sqlite3_bind_int64(stmt, 1, id);
        auto rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            _dispatcher->post([callback = std::move(callback)] {
                callback(RepositoryError{RepositoryErrorCode::Success, ""});
            });
        } else {
            _dispatcher->post([callback = std::move(callback), rc] {
                callback(RepositoryError{RepositoryErrorCode::DatabaseError,
                                         sqlite3_errstr(rc)});
            });
        }
        sqlite3_finalize(stmt);
    });
}

void HistoryRepository::deleteAll(HistoryDeletedCallback callback) {
    _dbManager->post([this, callback = std::move(callback)](sqlite3 *db) {
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare_v2(db, "DELETE FROM history", -1, &stmt, nullptr);

        auto rc = sqlite3_step(stmt);
        RepositoryError error;
        if (rc == SQLITE_DONE) {
            error = RepositoryError{RepositoryErrorCode::Success, ""};
        } else {
            error = RepositoryError{RepositoryErrorCode::DatabaseError,
                                    sqlite3_errstr(rc)};
        }
        _dispatcher->post([callback = std::move(callback), error = std::move(error)] {
            callback(error);
        });
        sqlite3_finalize(stmt);
    });
}
