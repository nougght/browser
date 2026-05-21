#ifndef IBROWSER_CORE_H
#define IBROWSER_CORE_H

#include "core/Event.h"
#include "core/Url.h"
#include "core/models.h"
#include "core/eventArgs.h"
#include "ICoreDispatcher.h"


// ядро браузера с встроенным циклом событий(event loop)
class IBrowserCore : public ICoreDispatcher {
public:
    // добавить задачу в очередь обработки
    virtual void loadTabs() = 0;
    virtual void createTab(Url url) = 0;
    virtual void createTab() = 0;
    virtual void closeTab(TabId id) = 0;
    virtual void changeActiveTab(TabId id) = 0;
    virtual void moveTab(TabId id, int newIndex) = 0;
    virtual void goForward(TabId id) = 0;
    virtual void goBack(TabId id) = 0;
    virtual void visitUrl(TabId id, Url url) = 0;
    virtual void changeTabUrl(TabId id, Url url) = 0;
    virtual void changeTabTitle(TabId id, std::string title) = 0;
    virtual void changeTabLoadingProgress(TabId id, int progress) = 0;
    virtual void setTabLoadingStatus(TabId id, bool isLoading) = 0;

    virtual void reloadTab(TabId id) = 0;
    virtual void loadHistory() = 0;


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



    Event<std::vector<HistoryEntry>> historyLoaded;
    Event<HistoryEntry> historyEntryAdded;
};


IBrowserCore* CreateBrowserCore();
#endif
