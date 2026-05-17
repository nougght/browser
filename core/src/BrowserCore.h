#ifndef BROWSER_CORE_H
#define BROWSER_CORE_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "core/interfaces/IBrowserCore.h"
#include "domain/Tab.h"
#include "storage/DatabaseManager.h"
#include "storage/repositories/HistoryRepository.h"
#include "services/history/HistoryService.h"
#include "services/tabs/TabManager.h"


// ядро браузера с встроенным циклом событий(event loop)
class BrowserCore : public IBrowserCore {
private:
    // запуск event loop
    void _run();

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

    std::unique_ptr<DatabaseManager> _dbManager;
    std::unique_ptr<HistoryRepository> _historyRepo;
    // отдельные сервисы ядра
    std::unique_ptr<TabManager> _tabManager;

    std::unique_ptr<HistoryService> _historyService;

    // (временно) список подписок на события из сервисов
    std::vector<std::unique_ptr<ISubscription>> _subs;

public:
    BrowserCore();
    ~BrowserCore();

    // добавить задачу в очередь обработки
    void post(std::function<void()> task) override;

    void loadTabs() override;
    void createTab(Url url) override;
    void createTab() override;
    void closeTab(TabId id) override;
    void changeActiveTab(TabId id) override;
    void moveTab(TabId id, int newIndex) override;
    void goForward(TabId id) override;
    void goBack(TabId id) override;
    void visitUrl(TabId id, Url url) override;
    void changeTabUrl(TabId id, Url url) override;
    void changeTabTitle(TabId id, std::string title) override;
    void changeTabLoadingProgress(TabId id, int progress) override;
    void setTabLoadingStatus(TabId id, bool isLoading) override;

    void reloadTab(TabId id) override;

    // прямые геттеры в основном классе не используются, вместо них - события
    // т.к. ядро в отдельном потоке

    Tab *getTab(TabId id);
    TabId getActiveTabId();
    const std::vector<TabId> &getTabsOrder();
    bool canGoBack(TabId id);
    bool canGoForward(TabId id);


};

#endif
