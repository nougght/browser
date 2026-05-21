#include "BrowserCore.h"
#include "services/tabs/TabManager.h"

IBrowserCore *CreateBrowserCore() { return new BrowserCore; }
BrowserCore::BrowserCore()
    : _dbManager(std::make_unique<DatabaseManager>("test.db")),
    _historyRepo(std::make_unique<HistoryRepository>(_dbManager.get(), this)),
    _historyService(std::make_unique<HistoryService>(_historyRepo.get())),

    _tabManager(std::make_unique<TabManager>()),
    _eventLoopWorker([this] { _run(); }) {
    post([this] {
        _subs.push_back(std::make_unique<Subscription<TabInfo>>(
            _tabManager->tabCreated.subscribe(
                [this](TabInfo tabInfo) { this->tabCreated.invoke(tabInfo); })));

        _subs.push_back(std::make_unique<Subscription<std::vector<TabInfo>>>(
            _tabManager->tabsLoaded.subscribe(
                [this](std::vector<TabInfo> tabs) { tabsLoaded.invoke(tabs); })));

        _subs.push_back(std::make_unique<Subscription<TabId>>(
            _tabManager->activeTabChanged.subscribe(
                [this](TabId id) { activeTabChanged.invoke(id); })));

        _subs.push_back(
            std::make_unique<Subscription<TabId>>(_tabManager->tabClosed.subscribe(
                [this](TabId id) { tabClosed.invoke(id); })));

        _subs.push_back(std::make_unique<Subscription<NavigationRequestedArgs>>(
            _tabManager->navigationRequested.subscribe(
                [this](NavigationRequestedArgs args) {
                    navigationRequested.invoke(args);
                })));

        _subs.push_back(std::make_unique<Subscription<NavigationRequestedArgs>>(
            this->navigationRequested.subscribe(
                [this](NavigationRequestedArgs args) {
                    _historyService->onNavigation(args);
                })));

        _subs.push_back(std::make_unique<Subscription<TabTitleChangedArgs>>(
            _tabManager->titleChanged.subscribe(
                [this](TabTitleChangedArgs args) { titleChanged.invoke(args); })));
        _subs.push_back(
            std::make_unique<Subscription<TabLoadingProgressChangedArgs>>(
                _tabManager->loadingProgressChanged.subscribe(
                    [this](TabLoadingProgressChangedArgs args) {
                  loadingProgessChanged.invoke(args);
                    })));
        _subs.push_back(std::make_unique<Subscription<TabLoadingStatusChangedArgs>>(
            _tabManager->loadingStatusChanged.subscribe(
                [this](TabLoadingStatusChangedArgs args) {
                    loadingStatusChanged.invoke(args);
                })));
        _subs.push_back(std::make_unique<Subscription<TabIconChangedArgs>>(
            _tabManager->iconChanged.subscribe(
                [this](TabIconChangedArgs args) { iconChanged.invoke(args); })));

        _subs.push_back(std::make_unique<Subscription<std::vector<HistoryEntry>>>(
            _historyService->historyLoaded.subscribe(
                [this](std::vector<HistoryEntry> history) {
                    std::cout << "\n browser core history loaded\n";
                    historyLoaded.invoke(history);
                })));

        _subs.push_back(std::make_unique<Subscription<HistoryEntry>>(
            _historyService->entryAdded.subscribe(
                [this](HistoryEntry entry) {
                    std::cout << "\n browser core history entry added\n";
                    historyEntryAdded.invoke(entry);
                })));
    });
}

BrowserCore::~BrowserCore() {
    // отключаем event loop поток
    _shutdownEventLoop();
}

void BrowserCore::_shutdownEventLoop() {
    _shouldStop = true;
    _condVar.notify_all();
    if (_eventLoopWorker.joinable()) {
        _eventLoopWorker.join();
    }
}

void BrowserCore::_run() {
    // выполняется в своем потоке пока не будет остановлен
    while (!_shouldStop) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            // ожидание внешней остановки или появления новой задачи
            _condVar.wait(lock, [this] {
                return this->_shouldStop || !(this->_tasks.empty());
            });
            if (_shouldStop)
                break;
            // перемещение данных в core поток
            task = std::move(_tasks.front());
            _tasks.pop();
        }
        // выполнение очередной задачи
        task();
    }
}

void BrowserCore::post(std::function<void()> task) {
    {
        // блокировка для доступа к очереди
        std::lock_guard<std::mutex> lock(_mutex);
        // добавляем задачу в очередь
        _tasks.push(std::move(task));
    }
    // уведомляем о появлении новой задачи
    _condVar.notify_one();
}

// TabManager *BrowserCore::getTabManager()
// {
//     return _tabManager.get();
// }

void BrowserCore::loadTabs() {
    post([this] { _tabManager->loadTabs(); });

    // post([this]()
    //       { tabsLoaded.invoke(_tabManager->getTabInfos()); });
}

void BrowserCore::loadHistory() {
    post([this]{_historyService->loadHistory();});
}
void BrowserCore::createTab(Url url) {
    post([this, url = std::move(url)] {
        TabId id = _tabManager->createTab(url);
        // TabInfo tabInfo = _tabManager->getTab(id)->toTabInfo();
        // tabCreated.invoke(tabInfo);
        // navigationCompleted.invoke(NavigationCompletedArgs{NavigationType::NewPage,
        // tabInfo});
    });
}

void BrowserCore::createTab() {
    post([this] { _tabManager->createTab(); });
}

void BrowserCore::closeTab(TabId id) {
    post([this, id] {
        if (!id.isValid())
            return;
        _tabManager->closeTab(id);
    });
}

void BrowserCore::visitUrl(TabId id, Url url) {
    post([this, id, url] {
        std::cerr << "\nVisitURL\n";
        if (!id.isValid())
            return;
        _tabManager->visitUrl(id, url);
        navigationRequested.invoke(NavigationRequestedArgs{
                                                           NavigationType::NewPage, _tabManager->getTab(id)->toTabInfo()});
        // urlVisited.invoke(UrlVisitedArgs{id, url});
    });
}

void BrowserCore::changeTabUrl(TabId id, Url url) {
    post([this, id, url] {
        if (!id.isValid())
            return;
        _tabManager->changeTabUrl(id, url);
        navigationRequested.invoke(NavigationRequestedArgs{
                                                           NavigationType::Redirect, _tabManager->getTab(id)->toTabInfo()});
    });
}

void BrowserCore::changeTabTitle(TabId id, std::string title) {
    post([this, id, title] {
        if (!id.isValid())
            return;
        _tabManager->changeTabTitle(id, title);
    });
}

void BrowserCore::setTabLoadingStatus(TabId id, bool isLoading) {
    post([this, id, isLoading] {
        if (!id.isValid())
            return;
        _tabManager->setTabLoadingStatus(id, isLoading);
    });
}

void BrowserCore::changeTabLoadingProgress(TabId id, int progress) {
    post([this, id, progress] {
        if (!id.isValid())
            return;
        _tabManager->changeTabLoadingProgress(id, progress);
    });
}

void BrowserCore::changeActiveTab(TabId id) {
    post([this, id] {
        if (!id.isValid())
            return;
        _tabManager->changeActiveTab(id);
    });
}

void BrowserCore::reloadTab(TabId id) {
    post([this, id] {
        if (!id.isValid())
            return;
        _tabManager->reloadTab(id);
    });
}

void BrowserCore::goBack(TabId id) {
    post([this, id] {
        if (!id.isValid())
            return;
        _tabManager->goBack(id);
    });
}
void BrowserCore::goForward(TabId id) {
    post([this, id] {
        if (!id.isValid())
            return;
        _tabManager->goForward(id);
    });
}

void BrowserCore::moveTab(TabId id, int newIndex) {
    post([this, id, newIndex] {
        if (!id.isValid())
            return;
        _tabManager->moveTab(id, newIndex);
    });
}

// --------
// unused

bool BrowserCore::canGoBack(TabId id) {
    if (!id.isValid())
        return false;
    return _tabManager->canGoBack(id);
}

bool BrowserCore::canGoForward(TabId id) {
    if (!id.isValid())
        return false;
    return _tabManager->canGoForward(id);
}

TabId BrowserCore::getActiveTabId() { return _tabManager->getActiveTabId(); }

Tab *BrowserCore::getTab(TabId id) {
    if (!id.isValid())
        return nullptr;
    return _tabManager->getTab(id);
}

const std::vector<TabId> &BrowserCore::getTabsOrder() {
    return _tabManager->getTabsOrder();
}
