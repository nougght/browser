#include "BookmarkService.h"

BookmarkService::BookmarkService(IBookmarkRepository *repo, TabManager *tabManager) : _repo(repo), _tabManager(tabManager) {}

void BookmarkService::_updateIndexAdd(size_t ind){
    if (ind >= _bookmarks.size()){
        return;
    }
    auto b = _bookmarks[ind];
    _idIndex[b.id] = ind;
    _urlIndex[b.url] = ind;
}

void BookmarkService::_updateIndexDelete(size_t ind) {
    if (ind >= _bookmarks.size()){
        return;
    }
    auto b = _bookmarks[ind];
    _idIndex.erase(b.id);
    _urlIndex.erase(b.url);
}

void BookmarkService::loadBookmarks() {
    _repo->getBookmarks([this](std::vector<Bookmark> bookmarks) {
        _bookmarks.insert(_bookmarks.end(), bookmarks.begin(), bookmarks.end());
        for (int i = 0; i < bookmarks.size(); ++i) {
            _updateIndexAdd(i);
        }
        std::printf("\nbookmarks service loaded bookmarks: size = %d\n", _bookmarks.size());
        bookmarksLoaded.invoke(bookmarks);
    });
}


void BookmarkService::switchActiveTabBookmark(){
    std::cerr<< "\nbookmarks service switch bookmark\n";
    auto tab = _tabManager->getTab(_tabManager->getActiveTabId());
    if (auto it = _urlIndex.find(tab->getUrl().toStdString()); it == _urlIndex.end()) {
        auto b = Bookmark{-1, tab->getUrl().toStdString(), tab->getTitle()};
        _repo->addBookmark(b, [this](Bookmark bookmark){
            _bookmarks.insert(_bookmarks.end(), bookmark);
            _updateIndexAdd(_bookmarks.size() - 1);
            std::printf("\nbookmarks service added bookmark: %d\n", _bookmarks.size() - 1);
            bookmarkAdded.invoke(bookmark);
        });
    } else {
        auto ind = it->second;
        _repo->deleteBookmark(_bookmarks[ind].id);
        _updateIndexDelete(it->second);
        _bookmarks.erase(_bookmarks.begin() + ind);
        std::printf("\nbookmarks service deleted bookmark: %d\n", ind);
        bookmarkDeleted.invoke(ind);
    }
}

void BookmarkService::deleteBookmark(int64_t id) {
    if (_idIndex.count(id) == 0) {
        return;
    }

    auto ind = _idIndex[id];
    _updateIndexDelete(ind);
    _bookmarks.erase(_bookmarks.begin() + ind);
    std::printf("\nbookmarks service deleted bookmark\n");
    bookmarkDeleted.invoke(ind);

}
