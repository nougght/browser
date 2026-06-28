#include "CoreAdapter.h"
#include <QDebug>
#include <memory>

// subscribing to core events and emit qt signals
void CoreAdapter::_setupEvents() {
    _subscriptions.push_back(std::make_unique<Subscription<std::vector<TabInfo>>>(
        _core->tabsLoaded.subscribe([this](std::vector<TabInfo> tabs) {
            QMetaObject::invokeMethod(this, [this, tabs = std::move(tabs)]() {
                emit this->tabsLoaded(tabs);
            });
        })));

    _subscriptions.push_back(std::make_unique<Subscription<TabInfo>>(
        _core->tabCreated.subscribe([this](TabInfo tabInfo) {
            QMetaObject::invokeMethod(
                this, [this, tabInfo]() { emit this->tabCreated(tabInfo); },
                Qt::QueuedConnection);
        })));

    _subscriptions.push_back(std::make_unique<Subscription<TabId>>(
        _core->tabClosed.subscribe([this](auto id) {
            QMetaObject::invokeMethod(
                this, [this, id]() { emit this->tabClosed(id); },
                Qt::QueuedConnection);
        })));

    _subscriptions.push_back(std::make_unique<Subscription<void>>(
        _core->lastTabClosed.subscribe([this]() {
            QMetaObject::invokeMethod(
                this, [this]() { emit this->lastTabClosed(); },
                Qt::QueuedConnection);
        })));

    _subscriptions.push_back(std::make_unique<Subscription<TabId>>(
        _core->activeTabChanged.subscribe([this](TabId id) {
            QMetaObject::invokeMethod(
                this, [this, id]() { emit this->activeTabChanged(id); },
                Qt::QueuedConnection);
        })));

    _subscriptions.push_back(
        std::make_unique<Subscription<NavigationRequestedArgs>>(
            _core->navigationRequested.subscribe(
                [this](NavigationRequestedArgs args) {
                QMetaObject::invokeMethod(
                        this,
                        [this, args]() { emit this->navigationRequested(args); },
                        Qt::QueuedConnection);
                })));

    _subscriptions.push_back(std::make_unique<Subscription<TabTitleChangedArgs>>(
        _core->titleChanged.subscribe([this](TabTitleChangedArgs args) {
            QMetaObject::invokeMethod(
                this, [this, args]() {
                    std::printf("adapter on tab title changed");
                    emit this->titleChanged(args); },
                Qt::QueuedConnection);
        })));

    _subscriptions.push_back(
        std::make_unique<Subscription<TabLoadingStatusChangedArgs>>(
            _core->loadingStatusChanged.subscribe(
                [this](TabLoadingStatusChangedArgs args) {
                QMetaObject::invokeMethod(
                        this,
                        [this, args]() { emit this->loadingStatusChanged(args); },
                        Qt::QueuedConnection);
                })));

    _subscriptions.push_back(
        std::make_unique<Subscription<TabLoadingProgressChangedArgs>>(
            _core->loadingProgessChanged.subscribe(
                [this](TabLoadingProgressChangedArgs args) {
                QMetaObject::invokeMethod(
                        this,
                        [this, args]() { emit this->loadingProgressChanged(args); },
                        Qt::QueuedConnection);
                })));


    _subscriptions.push_back(
        std::make_unique<Subscription<std::vector<HistoryEntry>>>(
            _core->historyLoaded.subscribe(
                [this](std::vector<HistoryEntry> history) {
                QMetaObject::invokeMethod(
                        this,
                        [this, history]() {
                            qDebug() << "\n adapter history loaded\n";
                            emit this->historyLoaded(history);
                        },
                        Qt::QueuedConnection);
                })));

    _subscriptions.push_back(std::make_unique<Subscription<HistoryEntry>>(
        _core->historyEntryAdded.subscribe([this](HistoryEntry entry) {
            QMetaObject::invokeMethod(
                this,
                [this, entry]() {
                    qDebug() << "\n adapter history entry added\n";
                    emit this->historyEntryAdded(entry);
                },
                Qt::QueuedConnection);
        })));

    _subscriptions.push_back(std::make_unique<Subscription<HistoryEntry>>(
        _core->historyEntryUpdated.subscribe([this](HistoryEntry entry) {
            QMetaObject::invokeMethod(
                this, [this, entry]() { emit this->historyEntryUpdated(entry); },
                Qt::QueuedConnection);
        })));
        
    _subscriptions.push_back(std::make_unique<Subscription<int64_t>>(
        _core->historyEntryDeleted.subscribe([this](int64_t id) {
            QMetaObject::invokeMethod(
                this, [this, id]() { emit this->historyEntryDeleted(id); },
                Qt::QueuedConnection);
        })));
    _subscriptions.push_back(std::make_unique<Subscription<void>>(
        _core->historyCleared.subscribe([this]() {
            QMetaObject::invokeMethod(
                this, [this]() { emit this->historyCleared(); },
                Qt::QueuedConnection);
        })));

    _subscriptions.push_back(
        std::make_unique<Subscription<std::vector<Bookmark>>>(
            _core->bookmarksLoaded.subscribe([this](auto bookmarks) {
            QMetaObject::invokeMethod(
                this,
                [this, bookmarks]() { emit this->bookmarksLoaded(bookmarks); },
                    Qt::QueuedConnection);
            })));
    _subscriptions.push_back(std::make_unique<Subscription<Bookmark>>(
        _core->bookmarkAdded.subscribe([this](auto bookmark) {
            QMetaObject::invokeMethod(
                this, [this, bookmark]() { emit this->bookmarkAdded(bookmark); },
                Qt::QueuedConnection);
        })));
    _subscriptions.push_back(std::make_unique<Subscription<int64_t>>(
        _core->bookmarkDeleted.subscribe([this](int64_t id) {
            QMetaObject::invokeMethod(
                this,
                [this, id]() {
                    qDebug() << "\n adapter bookmark deleted\n";
                    emit this->bookmarkDeleted(id);
                },
                Qt::QueuedConnection);
        })));
}


void CoreAdapter::loadHistory() { _core->loadHistory(); }
void CoreAdapter::deleteHistoryEntry(int64_t id) { _core->deleteHistoryEntry(id); }
void CoreAdapter::clearHistory() { _core->clearHistory(); }


void CoreAdapter::loadBookmarks() { _core->loadBookmarks(); }
void CoreAdapter::switchActiveTabBookmark() {
    qDebug() << "\n adapter switch tab bookmark\n";
    _core->switchActiveTabBookmark();
}
void CoreAdapter::addBookmark(Bookmark bookmark) { _core->addBookmark(bookmark); }
void CoreAdapter::deleteBookmark(int64_t id) { _core->deleteBookmark(id); }


void CoreAdapter::loadTabs() { _core->loadTabs(); }
void CoreAdapter::createTab(QUrl url) { _core->createTab(convert(url)); }

void CoreAdapter::createTab() { _core->createTab(); }

void CoreAdapter::closeTab(TabId id) { _core->closeTab(id); }

void CoreAdapter::changeActiveTab(TabId id) { _core->changeActiveTab(id); }

void CoreAdapter::moveTab(TabId id, int newIndex) {
    _core->moveTab(id, newIndex);
}

void CoreAdapter::goForward(TabId id) { _core->goForward(id); }

void CoreAdapter::goBack(TabId id) { _core->goBack(id); }

void CoreAdapter::handleSearchQuery(TabId id, std::string query) {
    // _core->visitUrl(id, convert(url));
    _core->handleSearchQuery(id, query);
}

void CoreAdapter::openInternalPage(InternalPageType type, bool isNewTab) {
    _core->openInternalPage(type, isNewTab);
}

void CoreAdapter::changeTabUrl(TabId id, QUrl url) {
    _core->onEngineUrlChanged(id, convert(url));
}

void CoreAdapter::changeTabTitle(TabId id, QString title) {
    _core->onEngineTitleChanged(id, title.toStdString());
}

void CoreAdapter::changeTabLoadingProgress(TabId id, int progress) {
    _core->changeTabLoadingProgress(id, progress);
}

void CoreAdapter::setTabLoadingStatus(TabId id, bool isLoading) {
    _core->setTabLoadingStatus(id, isLoading);
}

void CoreAdapter::reloadTab(TabId id) { _core->reloadTab(id); }
