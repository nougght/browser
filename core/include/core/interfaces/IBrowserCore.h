#ifndef IBROWSER_CORE_H
#define IBROWSER_CORE_H

#include "core/Event.h"
#include "core/Url.h"
#include "core/models.h"
#include "core/eventArgs.h"
#include "ICoreDispatcher.h"


// TODO: добавить возврат ошибок

// ядро браузера с встроенным циклом событий(event loop)
class IBrowserCore : public ICoreDispatcher {
public:
    // tabs
    virtual void loadTabs() = 0;
    virtual void createTab(Url url) = 0;
    virtual void createTab() = 0;
    virtual void closeTab(TabId id) = 0;
    virtual void changeActiveTab(TabId id) = 0;
    virtual void moveTab(TabId id, int newIndex) = 0;
    virtual void goForward(TabId id) = 0;
    virtual void goBack(TabId id) = 0;
    virtual void handleSearchQuery(TabId id, std::string query) = 0;
    virtual void openInternalPage(InternalPageType type, bool isNewTab = true) = 0;
    // virtual void visitUrl(TabId id, Url url) = 0;

    virtual void onEngineUrlChanged(TabId id, Url url) = 0;
    virtual void onEngineTitleChanged(TabId id, std::string title) = 0;
    

    virtual void changeTabLoadingProgress(TabId id, int progress) = 0;
    virtual void setTabLoadingStatus(TabId id, bool isLoading) = 0;
    virtual void reloadTab(TabId id) = 0;

    // history
    virtual void loadHistory() = 0;
    virtual void deleteHistoryEntry(int64_t id) = 0;
    virtual void clearHistory() = 0;

    // bookmarks
    virtual void loadBookmarks() = 0;
    virtual void switchActiveTabBookmark() = 0;
    virtual void addBookmark(Bookmark bookmark) = 0;
    virtual void deleteBookmark(int64_t id) = 0;

    // события ядра

    // список вкладок
    Event<std::vector<TabInfo>> tabsLoaded;

    // создана новая вкладка
    Event<TabInfo> tabCreated;

    // переходы в рамках одной вкладки
    Event<NavigationRequestedArgs> navigationRequested;

    Event<TabTitleChangedArgs> titleChanged;

    Event<TabLoadingProgressChangedArgs> loadingProgessChanged;

    Event<TabIconChangedArgs> iconChanged;

    Event<TabLoadingStatusChangedArgs> loadingStatusChanged;

    // вкладка перемещена
    Event<TabMovedArgs> tabMoved;

    // вкладка закрыта
    Event<TabId> tabClosed;

    // новая активная вкладка
    Event<TabId> activeTabChanged;

    // все вкладки закрыты
    Event<void> lastTabClosed;

    // перезагрузка вкладки
    Event<TabId> tabReloaded;


    // history events

    Event<std::vector<HistoryEntry>> historyLoaded;
    Event<HistoryEntry> historyEntryAdded;
    Event<HistoryEntry> historyEntryUpdated;
    Event<int64_t> historyEntryDeleted;
    Event<void> historyCleared;

    // bookmarks events
    Event<std::vector<Bookmark>> bookmarksLoaded;
    Event<Bookmark> bookmarkAdded;
    Event<int64_t> bookmarkDeleted;
};


IBrowserCore* CreateBrowserCore();
#endif
