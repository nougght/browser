#ifndef BOOKMARKSERVICE_H
#define BOOKMARKSERVICE_H

#include "interfaces/IBookmarksRepository.h"
#include "services/tabs/TabManager.h"

class BookmarkService
{
private:
    IBookmarkRepository *_repo;
    TabManager *_tabManager;
    std::vector<Bookmark> _bookmarks;
    std::unordered_map<int64_t, size_t> _idIndex;
    std::unordered_map<std::string, size_t> _urlIndex;

    void _updateIndexAdd(size_t ind);
    void _updateIndexDelete(size_t ind);


public:
    BookmarkService(IBookmarkRepository *repo, TabManager *tabManager);
    void loadBookmarks();
    void switchActiveTabBookmark();
    void addBookmark(Bookmark bookmark);
    void deleteBookmark(int64_t id);

    Event<std::vector<Bookmark>> bookmarksLoaded;
    Event<Bookmark> bookmarkAdded;
    Event<int64_t> bookmarkDeleted;
};

#endif // BOOKMARKSERVICE_H
