#include "BookmarkController.h"

BookmarkController::BookmarkController(CoreAdapter *coreAdapter, UIContext *ctx)
    : _coreAdapter(coreAdapter), _ctx(ctx) {
    _setupEvents();
}

void BookmarkController::_setupEvents() {
    connect(_coreAdapter, &CoreAdapter::bookmarksLoaded, this,
            &BookmarkController::onBookmarksLoaded);
    connect(_coreAdapter, &CoreAdapter::bookmarkAdded, this,
            &BookmarkController::onBookmarkAdded);
    connect(_coreAdapter, &CoreAdapter::bookmarkDeleted, this,
            &BookmarkController::onBookmarkDeleted);
}

void BookmarkController::onBookmarksLoaded(std::vector<Bookmark> bookmarks) {
    qDebug() << "\n bookmark controller bookmarks loaded\n";
    _ctx->getBookmarkModel()->addAll(bookmarks);
    emit bookmarksLoaded(std::move(bookmarks));
}

void BookmarkController::_syncActiveTabBookmark(std::string url,
                                                bool isBookmarked) {

    qDebug() << "\nBC sync url: " << url << "\n";
    if (url == _ctx->activeTabURL()) {
        emit switchActiveTabBookmark(isBookmarked);
        qDebug() << "\nBC sync active tab bookmark = " << isBookmarked << "\n";
    }
}

void BookmarkController::onBookmarkAdded(Bookmark bookmark) {
    qDebug() << "\n bookmark controller bookmark added\n";
    _ctx->getBookmarkModel()->add(bookmark);
    _syncActiveTabBookmark(bookmark.url, true);
    // emit bookmarkAdded(std::move(bookmark));
}

void BookmarkController::onBookmarkDeleted(size_t index) {
    auto url = _ctx->getBookmarkModel()->getUrl(index);
    qDebug() << "\n bookmark controller bookmark deleted: " << index
             << " url = " << url << "\n";
    _ctx->getBookmarkModel()->remove(index);
    _syncActiveTabBookmark(url, false);
    // emit bookmarkRemoved(index);
}

void BookmarkController::onActiveTabBookmakToggled() {
    _coreAdapter->switchActiveTabBookmark();
    qDebug() << "\n bookmark controller on bookmark toggled\n";
}
