#ifndef BOOKMARKREPOSITORY_H
#define BOOKMARKREPOSITORY_H
#include "core/interfaces/ICoreDispatcher.h"
#include "interfaces/IBookmarksRepository.h"
#include "storage/DatabaseManager.h"

class BookmarkRepository : public IBookmarkRepository {
private:
    DatabaseManager *_dbManager;
    ICoreDispatcher *_dispatcher;

public:
    BookmarkRepository(DatabaseManager *dbManager, ICoreDispatcher *dispatcher);
    void addBookmark(Bookmark &bookmark, BookmarkAddedCallback callback) override;
    void deleteBookmark(int64_t id, BookmarkDeletedCallback callback) override;
    void getBookmarks(BookmarkGetCallback callback) override;
};

#endif // BOOKMARKREPOSITORY_H
