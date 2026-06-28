#include "HistoryService.h"

HistoryService::HistoryService(IHistoryRepository *historyRepo)
    : _historyRepo(historyRepo) {}



void HistoryService::_addVisit(TabId tabId, Url url, std::string title) {
    _tabUrlBuffer[tabId] = url;
    _tabTitleBuffer[tabId] = title;
    auto entry = HistoryEntry{0, url.toStdString(), title, 0};
    _historyRepo->addVisit(entry, [this, tabId, url, title](std::optional<HistoryEntry> createdEntry, RepositoryError error) {
        if (error.code == RepositoryErrorCode::Success) {
            if (createdEntry) {
                std::printf("service add visit success\n");
                _loadingTabHistoryId[tabId] = createdEntry.value().id;

                // если во время сохранения url и title были изменены, то обновить историю
                if (_tabUrlBuffer.find(tabId) != _tabUrlBuffer.end() && _tabUrlBuffer[tabId] != url) {
                    _updateUrl(tabId, _tabUrlBuffer[tabId]);
                }
                if (_tabTitleBuffer.find(tabId) != _tabTitleBuffer.end() && _tabTitleBuffer[tabId] != title) {
                    _updateTitle(tabId, _tabTitleBuffer[tabId]);
                }
                entryAdded.invoke(createdEntry.value());
            } else {
                std::printf("add visit result is absent");
            }
        } else {
            std::printf("service add visit error: %s\n", error.message.c_str());
        }
    });
}

void HistoryService::onNavigation(NavigationCompletedArgs args) {
    // don't add internal pages to history
    if (args.tabInfo.url.isInternal())
    {
        return;
    }
    switch (args.type) {
        case NavigationType::NewPage:
            _addVisit(args.tabInfo.id, args.tabInfo.url, args.tabInfo.title);
            break;
        case NavigationType::Redirect:
            _updateUrl(args.tabInfo.id, args.tabInfo.url);
            break;
        default:
            break;
    }
}

void HistoryService::_clearTabBuffer(TabId id){
    _loadingTabHistoryId.erase(id);
    _tabUrlBuffer.erase(id);
    _tabTitleBuffer.erase(id);
}

void HistoryService::onTabTitleChanged(TabId id, std::string title){
    _updateTitle(id, title);
}

void HistoryService::onTabLoaded(TabId id){
    _clearTabBuffer(id);
}

void HistoryService::onTabClosed(TabId id){
    _clearTabBuffer(id);
}

void HistoryService::_updateTitle(TabId id, std::string title){
    auto it = _loadingTabHistoryId.find(id);
    if (it != _loadingTabHistoryId.end()) {
        auto historyId = it->second;
        auto entry = HistoryEntry{historyId, "", title};
        _historyRepo->updateTitle(historyId, title, [this, historyId, title](RepositoryError error) {
            if (error.code == RepositoryErrorCode::Success) {
                std::printf("service update entry success\n");
                entryUpdated.invoke(HistoryEntry{historyId, "", title});
            } else {
                std::printf("service update entry error: %s\n", error.message.c_str());
            }
        });
    // обновляем буфер title
    } else if (_tabTitleBuffer.find(id) != _tabTitleBuffer.end() && _tabTitleBuffer[id] != title) {
        _tabTitleBuffer[id] = title;
    }
}

void HistoryService::_updateUrl(TabId id, Url url){
    auto it = _loadingTabHistoryId.find(id);
    if (it != _loadingTabHistoryId.end()) {
        auto historyId = it->second;
        auto entry = HistoryEntry{historyId, url.toStdString()};
        _historyRepo->updateUrl(historyId, url.toStdString(), [this, historyId, url](RepositoryError error) {
            if (error.code == RepositoryErrorCode::Success) {
                std::printf("service update entry success\n");
                entryUpdated.invoke(HistoryEntry{historyId, url.toStdString(), ""});
            } else {
                std::printf("service update entry error: %s\n", error.message.c_str());
            }
        });
    // обновляем буфер url
    } else if (_tabUrlBuffer.find(id) != _tabUrlBuffer.end() && _tabUrlBuffer[id] != url) {
        _tabUrlBuffer[id] = url;
    }
}

void HistoryService::loadHistory() {
    _historyRepo->getHistory([this](std::vector<HistoryEntry> historyList, RepositoryError error) {
        std::cerr << "\nservice load history\n";
        if (error.code == RepositoryErrorCode::Success) {
            std::printf("service load history success\n");
            this->historyLoaded.invoke(historyList);
        } else {
            std::printf("service load history error: %s\n", error.message.c_str());
        }
    });
}


void HistoryService::deleteEntry(int64_t id) {
    _historyRepo->deleteEntry(id, [this, id](RepositoryError error) {
        if (error.code == RepositoryErrorCode::Success) {
            std::printf("service delete entry success, id: %lld\n", id);
            entryDeleted.invoke(id);
        } else {
            std::printf("service delete entry error: %s\n", error.message.c_str());
        }
    });
}

void HistoryService::clearHistory() {
    _historyRepo->deleteAll([this](RepositoryError error) {
        if (error.code == RepositoryErrorCode::Success) {
            std::printf("service clear history success\n");
            historyCleared.invoke();
        } else {
            std::printf("service clear history error: %s\n", error.message.c_str());
        }
    });
}
