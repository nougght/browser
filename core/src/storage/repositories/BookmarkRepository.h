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
    void addBookmark(Bookmark &bookmark, std::function<void(Bookmark)>) override;
    void deleteBookmark(int64_t id) override;
    void getBookmarks(std::function<void(std::vector<Bookmark>)> callback) override;
};

#endif // BOOKMARKREPOSITORY_H
