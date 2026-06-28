#ifndef HISTOR_SERVICE_H
#define HISTOR_SERVICE_H

#include "interfaces/IHistoryRepository.h"
#include "core/eventArgs.h"
#include "core/Event.h"

class HistoryService
{
private:
    // TODO: добавить хранение части истории в памяти
    std::unordered_map<TabId, int64_t> _loadingTabHistoryId;
    IHistoryRepository *_historyRepo;
public:
    HistoryService(IHistoryRepository *historyRepo);

    // обработчики событий
    void onNavigation(NavigationRequestedArgs args);
    void onTabLoaded(TabId id);
    void onTabRedirected(TabId id, Url url);
    void onTabTitleChanged(TabId id, std::string title);

    void loadHistory();
    void deleteEntry(int64_t id);
    void clearHistory();


    Event<std::vector<HistoryEntry>> historyLoaded;
    Event<HistoryEntry> entryAdded;
    Event<HistoryEntry> entryUpdated;
    Event<int64_t> entryDeleted;
    Event<void> historyCleared;
};
#endif
