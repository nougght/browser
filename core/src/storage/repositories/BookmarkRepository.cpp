#include "BookmarkRepository.h"
#include <iostream>

BookmarkRepository::BookmarkRepository(DatabaseManager *dbManager,
                                       ICoreDispatcher *dispatcher)
    : _dbManager(dbManager), _dispatcher(dispatcher) {}

void BookmarkRepository::addBookmark(Bookmark &bookmark,
                                     std::function<void(Bookmark)> callback) {
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
        if (rc == SQLITE_ROW) {
            std::cerr << "\nsuccessful added bookmark\n";
            bookmark.id = sqlite3_column_int(stmt, 0);
            _dispatcher->post([callback = std::move(callback),
                               result = std::move(bookmark)] { callback(result); });
        } else {
            std::cerr << "insert failed: " << sqlite3_errstr(rc) << "\n";
        }
        sqlite3_finalize(stmt);
    });
}

void BookmarkRepository::deleteBookmark(int64_t id) {
    _dbManager->post([id](sqlite3 *db) {
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare_v2(db, "DELETE FROM bookmarks WHERE bookmarks.id = ?", -1,
                           &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);
        int rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE) {
            std::printf("\nsuccessful deleted bookmark: %d\n", id);
        } else {
            std::printf("delete failed: %s\n", sqlite3_errstr(rc));
        }
        sqlite3_finalize(stmt);
    });
}

void BookmarkRepository::getBookmarks(
    std::function<void(std::vector<Bookmark>)> callback) {
    _dbManager->post([this, callback](sqlite3 *db) {
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare_v2(db, "SELECT * FROM bookmarks", -1, &stmt, nullptr);
        std::vector<Bookmark> result;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int64_t id = sqlite3_column_int64(stmt, 0);
            std::string url = (const char *)sqlite3_column_text(stmt, 1);
            std::string title = (const char *)sqlite3_column_text(stmt, 2);
            int64_t created_at = sqlite3_column_int64(stmt, 4);
            result.push_back(Bookmark{id, url, title});
        }

        sqlite3_finalize(stmt);
        _dispatcher->post(
            [callback = std::move(callback), result = std::move(result)] {
                std::printf("bookmarks result size = %d", result.size());
                callback(result);
        });
    });
}
