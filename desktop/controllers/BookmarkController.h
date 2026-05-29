#ifndef BOOKMARK_CONTROLLER_H
#define BOOKMARK_CONTROLLER_H

#include <QObject>
#include <core/Identifier.h>
#include <core/models.h>
#include "adapter/CoreAdapter.h"
#include "models/bookmarkmodel.h"
#include "models/UIContext.h"


class BookmarkController : public QObject
{
    Q_OBJECT

public:
    BookmarkController(CoreAdapter *coreAdapter, UIContext *ctx);

    // slots for core signals
    void onBookmarksLoaded(std::vector<Bookmark>);
    void onBookmarkAdded(Bookmark bookmark);
    void onBookmarkDeleted(size_t index);


    // slots for ui signals
    void onActiveTabBookmakToggled();

signals:
    void bookmarksLoaded(std::vector<Bookmark> bookmarks);
    void switchActiveTabBookmark(bool isBookmarked);

private:

    void _setupEvents();
    void _syncActiveTabBookmark(std::string url, bool isBookmarked);

    CoreAdapter *_coreAdapter;
    UIContext *_ctx;
};

#endif
