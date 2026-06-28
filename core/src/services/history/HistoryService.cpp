#include "HistoryService.h"

HistoryService::HistoryService(IHistoryRepository *historyRepo)
    : _historyRepo(historyRepo) {}

void HistoryService::onNavigation(NavigationRequestedArgs args) {
    if (args.type == NavigationType::NewPage) {
        if (args.tabInfo.url.isInternal())
        {
            // return;
        }
        std::cerr << "history service add visit\n";
        auto entry = HistoryEntry{0, args.tabInfo.url.toStdString(), args.tabInfo.title, 0};
        _historyRepo->addVisit(entry, 
        [this](std::optional<HistoryEntry> entry, RepositoryError error) {
            if (error.code == RepositoryErrorCode::Success) {
                if (entry) {
                    std::printf("service add visit success\n");
                    entryAdded.invoke(entry.value());
                } else {
                    std::printf("add visit result is absent");
                }
            } else {
                std::printf("service add visit error: %s\n", error.message.c_str());
            }
        });
    }
}

void HistoryService::onTabLoaded(TabId id){
    _loadingTabHistoryId.erase(id);
}

void HistoryService::onTabTitleChanged(TabId id, std::string title){
    auto it = _loadingTabHistoryId.find(id);
    if (it != _loadingTabHistoryId.end()) {
        auto historyId = it->second;
        auto entry = HistoryEntry{historyId, title};
        _historyRepo->updateEntry(entry, [this, entry](RepositoryError error) {
            if (error.code == RepositoryErrorCode::Success) {
                std::printf("service update entry success\n");
                entryUpdated.invoke(entry);
            } else {
                std::printf("service update entry error: %s\n", error.message.c_str());
            }
        });
    } else {
        std::printf("service update entry error: tab not found");
    }
}

void HistoryService::onTabRedirected(TabId id, Url url){
    auto it = _loadingTabHistoryId.find(id);
    if (it != _loadingTabHistoryId.end()) {
        auto historyId = it->second;
        auto entry = HistoryEntry{historyId, url.toStdString()};
        _historyRepo->updateEntry(entry, [this, entry](RepositoryError error) {
            if (error.code == RepositoryErrorCode::Success) {
                std::printf("service update entry success\n");
                entryUpdated.invoke(entry);
            } else {
                std::printf("service update entry error: %s\n", error.message.c_str());
            }
        });
    } else {
        std::printf("service update entry error: tab not found");
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
