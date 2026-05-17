#include "HistoryService.h"

HistoryService::HistoryService(IHistoryRepository *historyRepo)
    : _historyRepo(historyRepo) {}

void HistoryService::onNavigation(NavigationRequestedArgs args) {
    if (args.type == NavigationType::NewPage) {
        std::cout<<"history service add visit\n";
        _historyRepo->addVisit(
            HistoryEntry{0, args.tabInfo.url.toStdString(), args.tabInfo.title, 0});
    }
}
