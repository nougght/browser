#ifndef BROWSER_CORE_H
#define BROWSER_CORE_H

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>


#include "Tab.h"
#include "Event.h"
#include "Url.h"

class HistoryService;
class TabManager;

// ядро браузера с встроенным циклом событий(event loop)
class BrowserCore
{
private:

    // запуск event loop
    void _run();

    // добавить событие в очередь обработки
    void _post(std::function<void()> task);

    // отключение event loop
    void _shutdownEventLoop();


    // 
    std::thread _eventLoopWorker;

    // очередь задач(событий)
    std::queue<std::function<void()>> _tasks;

    // потокобезопасный доступ к очереди

    std::mutex _mutex;
    std::condition_variable _condVar;

    // безопасная переменная для остановки event loop
    std::atomic<bool> _shouldStop;



    // отдельные сервисы ядра
    std::unique_ptr<TabManager> _tabManager;
    // std::unique_ptr<HistoryService> _historyService;

    // (временно) список подписок на события из сервисов
    std::vector<std::unique_ptr<ISubscription>> _subs;
public:
    BrowserCore();
    ~BrowserCore();


    
    void loadTabs();
    void createTab(Url url);
    void createTab();
    void closeTab(TabId id);
    void changeActiveTab(TabId id);
    void moveTab(TabId id, int newIndex);
    void goForward(TabId id);
    void goBack(TabId id);
    void visitUrl(TabId id, Url url);
    void changeTabUrl(TabId id, Url url);
    void changeTabTitle(TabId id, std::string title);
    void changeTabLoadingProgress(TabId id, int progress);
    void setTabLoadingStatus(TabId id, bool isLoading);

    void reloadTab(TabId id);


    // прямые геттеры в основном классе не используются, вместо них - события
    // т.к. ядро в отдельном потоке

    Tab *getTab(TabId id);
    TabId getActiveTabId();
    const std::vector<TabId> &getTabsOrder();
    bool canGoBack(TabId id);
    bool canGoForward(TabId id);


    

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

     
};

#endif
