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
    for (auto i = ind + 1; i < _bookmarks.size(); ++i){
        _idIndex[_bookmarks[i].id] = i - 1;
        _urlIndex[_bookmarks[i].url] = i - 1;
    }
}

void BookmarkService::loadBookmarks() {
    _repo->getBookmarks([this](std::vector<Bookmark> bookmarks, RepositoryError error) {
        if (error.code == RepositoryErrorCode::Success) {
        auto oldSize = _bookmarks.size();
        _bookmarks.insert(_bookmarks.end(), bookmarks.begin(), bookmarks.end());
        for (size_t i = oldSize; i < _bookmarks.size(); ++i) {
            _updateIndexAdd(i);
        }
        std::printf("\nbookmarks service loaded bookmarks: size = %d\n", _bookmarks.size());
        bookmarksLoaded.invoke(bookmarks);
        } else {
            std::printf("service load bookmarks error: %s\n", error.message.c_str());
        }
    });
}


void BookmarkService::switchActiveTabBookmark(){
    std::cerr<< "\nbookmarks service switch bookmark\n";
    auto tab = _tabManager->getTab(_tabManager->getActiveTabId());
    if (auto it = _urlIndex.find(tab->getUrl().toStdString()); it == _urlIndex.end()) {
        auto b = Bookmark{-1, tab->getUrl().toStdString(), tab->getTitle()};
        addBookmark(b);
    } else {
        deleteBookmark(_bookmarks[it->second].id);
    }
}

void BookmarkService::addBookmark(Bookmark bookmark) {
    _repo->addBookmark(bookmark, [this](std::optional<Bookmark> bookmark, RepositoryError error) {
        if (error.code == RepositoryErrorCode::Success) {
            if (bookmark) {
                _bookmarks.insert(_bookmarks.end(), bookmark.value());
                _updateIndexAdd(_bookmarks.size() - 1);
                std::printf("bookmarks service added bookmark: %d\n", _bookmarks.size() - 1);
                bookmarkAdded.invoke(bookmark.value());
            } else {
                std::printf("add bookmark result is absent");
            }
        }
        else {
            std::printf("service add bookmark error: %s\n", error.message.c_str());
        }
    });
}

void BookmarkService::deleteBookmark(int64_t id) {
    if (_idIndex.count(id) == 0) {
        std::printf("BS delete bookmark not found");
        return;
    }
    _repo->deleteBookmark(id, [this, id](RepositoryError error) {
        if (error.code == RepositoryErrorCode::Success) {
            std::printf("BS delete bookmark success ");

            auto it = _idIndex.find(id);
            if (it != _idIndex.end()) {
                auto ind = it->second;
                _updateIndexDelete(ind);
                _bookmarks.erase(_bookmarks.begin() + ind);
            } else {
                std::printf("BS delete bookmark id not found in index\n");
            }
       
            std::printf("\nbookmarks service deleted bookmark\n");
            bookmarkDeleted.invoke(id);
        } else {
            std::printf("BS delete bookmark error: %s\n", error.message.c_str());
        }
    });
}
