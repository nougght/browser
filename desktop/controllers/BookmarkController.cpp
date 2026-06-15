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

void BookmarkController::onBookmarkDeleted(int64_t id) {
    auto optionalBookmark = _ctx->getBookmarkModel()->getBookmarkById(id);
    if (!optionalBookmark) {
        qDebug() << "bookmark controller bookmark deleted not found: " << id << "\n";
        return;
    }
    auto bookmark = optionalBookmark.value();
    qDebug() << "\n bookmark controller bookmark deleted: " << bookmark.id
             << " url = " << bookmark.url << "\n";
    _ctx->getBookmarkModel()->remove(bookmark.id);
    _syncActiveTabBookmark(bookmark.url, false);
    // emit bookmarkRemoved(index);
}




void BookmarkController::onActiveTabBookmakToggled() {
    _coreAdapter->switchActiveTabBookmark();
    qDebug() << "\n bookmark controller on bookmark toggled\n";
}

void BookmarkController::onBookmarkEntryClicked(int index) {
    auto optionalUrl = _ctx->getBookmarkModel()->getBookmarkUrlByIndex(index);
    if (!optionalUrl) {
        qDebug() << "\n bookmark controller bookmark entry clicked not found\n";
        return;
    }
    auto url = optionalUrl.value();
    _coreAdapter->createTab(QUrl(QString::fromStdString(url)));
    qDebug() << "\n bookmark controller bookmark entry clicked: " << index << " url = " << url << "\n";
}

void BookmarkController::onDeleteBookmarkClicked(int index) {
    auto optionalId = _ctx->getBookmarkModel()->getBookmarkIdByIndex(index);
    if (!optionalId) {
        qDebug() << "\n bookmark controller delete bookmark clicked not found\n";
        return;
    }
    auto id = optionalId.value();
    _coreAdapter->deleteBookmark(id);
    qDebug() << "\n bookmark controller delete bookmark clicked: " << index << " id = " << id << "\n";
}