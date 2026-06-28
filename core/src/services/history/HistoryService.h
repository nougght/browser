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
    // временное хранение при изменении url и title
    std::unordered_map<TabId, Url> _tabUrlBuffer;
    std::unordered_map<TabId, std::string> _tabTitleBuffer;

    IHistoryRepository *_historyRepo;
    void _clearTabBuffer(TabId id);
    void _addVisit(TabId tabId, Url url, std::string title);
    void _updateUrl(TabId tabId, Url url);
    void _updateTitle(TabId tabId, std::string title);
public:
    HistoryService(IHistoryRepository *historyRepo);

    // обработчики событий
    void onNavigation(NavigationCompletedArgs args);
    void onTabTitleChanged(TabId id, std::string title);
    void onTabLoaded(TabId id);
    void onTabClosed(TabId id);

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
