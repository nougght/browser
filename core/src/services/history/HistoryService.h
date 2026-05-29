#ifndef HISTOR_SERVICE_H
#define HISTOR_SERVICE_H

#include "interfaces/IHistoryRepository.h"
#include "core/eventArgs.h"
#include "core/Event.h"

class HistoryService
{
private:
    std::unordered_map<int64_t, size_t> _index;
    std::vector<std::unique_ptr<Bookmark>> _bookmarks;

    IHistoryRepository *_historyRepo;
public:
    HistoryService(IHistoryRepository *historyRepo);
    void onNavigation(NavigationRequestedArgs args);
    void loadHistory();
    Event<std::vector<HistoryEntry>> historyLoaded;
    Event<HistoryEntry> entryAdded;

};
#endif
