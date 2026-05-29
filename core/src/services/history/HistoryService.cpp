#include "HistoryService.h"

HistoryService::HistoryService(IHistoryRepository *historyRepo)
    : _historyRepo(historyRepo) {}

void HistoryService::onNavigation(NavigationRequestedArgs args) {
    if (args.type == NavigationType::NewPage) {
        std::cerr << "history service add visit\n";
        auto entry = HistoryEntry{0, args.tabInfo.url.toStdString(), args.tabInfo.title, 0};
        _historyRepo->addVisit(entry);
        entryAdded.invoke(entry);
    }
}

void HistoryService::loadHistory() {
    _historyRepo->getHistory([this](std::vector<HistoryEntry> historyList) {
        std::cerr << "\nservice load history\n";
        this->historyLoaded.invoke(historyList);
    });
}
