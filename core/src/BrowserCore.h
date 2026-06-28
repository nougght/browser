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
#include "storage/repositories/BookmarkRepository.h"
#include "services/history/HistoryService.h"
#include "services/bookmarks/BookmarkService.h"
#include "services/tabs/TabManager.h"

// TODO: add BrowserContext
// TODO: navigation service

// ядро браузера с встроенным циклом событий(event loop)
class BrowserCore : public IBrowserCore {
private:
    // запуск event loop
    void _run();

    // отключение event loop
    void _shutdownEventLoop();

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
    std::unique_ptr<BookmarkRepository> _bookmarkRepo;
    // отдельные сервисы ядра
    std::unique_ptr<TabManager> _tabManager;

    std::unique_ptr<HistoryService> _historyService;

    std::unique_ptr<BookmarkService> _bookmarkService;

    // (временно) список подписок на события из сервисов
    std::vector<std::unique_ptr<ISubscription>> _subs;

public:
    BrowserCore();
    ~BrowserCore();

    // добавить задачу в очередь обработки
    void post(std::function<void()> task) override;

    // tabs
    void loadTabs() override;
    void createTab(Url url, bool isBackground) override;
    void createTab(bool isBackground) override;
    void closeTab(TabId id) override;
    void changeActiveTab(TabId id) override;
    void moveTab(TabId id, int newIndex) override;
    void goForward(TabId id) override;
    void goBack(TabId id) override;
    void setSearchEngine(SearchEngine engine) override;
    void handleSearchQuery(TabId id, std::string query) override;
    void handleNavigationRequested(NavigationType type, TabId id, Url url) override;
    void openInternalPage(InternalPageType type, bool isNewTab = true) override;
    // void visitUrl(TabId id, Url url) override;

    void onEngineUrlChanged(TabId id, Url url) override;
    void onEngineTitleChanged(TabId id, std::string title) override;

    void changeTabLoadingProgress(TabId id, int progress) override;
    void setTabLoadingStatus(TabId id, bool isLoading) override;
    void reloadTab(TabId id) override;

    // history
    void loadHistory() override;
    void deleteHistoryEntry(int64_t id) override;
    void clearHistory() override;

    // bookmarks
    void loadBookmarks() override;
    void switchActiveTabBookmark() override;
    void addBookmark(Bookmark bookmark) override;
    void deleteBookmark(int64_t id) override;

    // прямые геттеры в основном классе не используются, вместо них - события
    // т.к. ядро в отдельном потоке

    Tab *getTab(TabId id);
    TabId getActiveTabId();
    const std::vector<TabId> &getTabsOrder();
    bool canGoBack(TabId id);
    bool canGoForward(TabId id);


};

#endif
