#ifndef HISTOR_SERVICE_H
#define HISTOR_SERVICE_H

#include "interfaces/IHistoryRepository.h"
#include "core/eventArgs.h"
#include "core/Event.h"

class HistoryService
{
private:
    // TODO: добавить хранение части истории в памяти

    IHistoryRepository *_historyRepo;
public:
    HistoryService(IHistoryRepository *historyRepo);
    void onNavigation(NavigationRequestedArgs args);
    void loadHistory();
    void deleteEntry(int64_t id);
    void clearHistory();


    Event<std::vector<HistoryEntry>> historyLoaded;
    Event<HistoryEntry> entryAdded;
    Event<int64_t> entryDeleted;
    Event<void> historyCleared;
};
#endif
