#include "BookmarkRepository.h"
#include <iostream>
#include <optional>

BookmarkRepository::BookmarkRepository(DatabaseManager *dbManager,
                                       ICoreDispatcher *dispatcher)
    : _dbManager(dbManager), _dispatcher(dispatcher) {}

void BookmarkRepository::addBookmark(Bookmark &bookmark,
                                     BookmarkAddedCallback callback) {
    _dbManager->post([this, bookmark = std::move(bookmark),
                      callback = std::move(callback)](sqlite3 *db) mutable {
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare_v2(db,
                           "INSERT INTO bookmarks(url, title) VALUES(?, ?)"
                           "RETURNING id",
                           -1, &stmt, nullptr);

        sqlite3_bind_text(stmt, 1, bookmark.url.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, bookmark.title.c_str(), -1, SQLITE_STATIC);

        int rc = sqlite3_step(stmt);
        RepositoryError error;
        if (rc == SQLITE_ROW) {
            bookmark.id = sqlite3_column_int(stmt, 0);
            error = RepositoryError{RepositoryErrorCode::Success, ""};
        } else {
            error = RepositoryError{RepositoryErrorCode::DatabaseError, sqlite3_errstr(rc)};
        }
        _dispatcher->post([callback = std::move(callback), bookmark = std::move(bookmark), error = std::move(error)] {
            callback(error.code == RepositoryErrorCode::Success ? std::make_optional(std::move(bookmark)) : std::nullopt, error);
        });
        sqlite3_finalize(stmt);
    });
}

void BookmarkRepository::deleteBookmark(int64_t id, BookmarkDeletedCallback callback) {
    _dbManager->post([this, id = std::move(id),
                      callback = std::move(callback)](sqlite3 *db) mutable {
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare_v2(db, "DELETE FROM bookmarks WHERE bookmarks.id = ?", -1,
                           &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);
        int rc = sqlite3_step(stmt);

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

void BookmarkRepository::getBookmarks(
    BookmarkGetCallback callback) {
    _dbManager->post([this, callback = std::move(callback)](sqlite3 *db) {
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare_v2(db, "SELECT * FROM bookmarks", -1, &stmt, nullptr);
        std::vector<Bookmark> result;
        int rc;
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int64_t id = sqlite3_column_int64(stmt, 0);
            std::string url = (const char *)sqlite3_column_text(stmt, 1);
            std::string title = (const char *)sqlite3_column_text(stmt, 2);
            int64_t created_at = sqlite3_column_int64(stmt, 4);
            result.push_back(Bookmark{id, url, title});
        }

        sqlite3_finalize(stmt);
        RepositoryError error;
        if (rc != SQLITE_DONE) {
            error = RepositoryError{RepositoryErrorCode::DatabaseError,
                                    sqlite3_errstr(rc)};
        } else {
            error = RepositoryError{RepositoryErrorCode::Success, ""};
        }
        _dispatcher->post(
            [callback = std::move(callback), result = std::move(result), error = std::move(error)] {
                std::printf("bookmarks result size = %d", result.size());
                callback(std::move(result), std::move(error));
        });
    });
}
