#include "TabManager.h"
#include <algorithm>
#include <iterator>

TabManager::TabManager() {}

void TabManager::loadTabs() {
    // createTab();
    TabId id = _idGenerator.create();
    auto tab = std::make_unique<Tab>(id, _initialTabUrl);
    _tabs.emplace(id, std::move(tab));

    _tabsOrder.push_back(id);
    tabsLoaded.invoke(getTabInfos());
    changeActiveTab(id);
}

TabId TabManager::createTab(Url url) {
    TabId id = _idGenerator.create();
    auto tab = std::make_unique<Tab>(id, std::move(url));
    _tabs.emplace(id, std::move(tab));

    _tabsOrder.push_back(id);
    tabCreated.invoke(getTab(id)->toTabInfo());
    changeActiveTab(id);
    return id;
}

TabId TabManager::createTab() { return createTab(_initialTabUrl); }

void TabManager::closeTab(TabId id) {
    auto it = std::find(_tabsOrder.begin(), _tabsOrder.end(), id);
    if (it != _tabsOrder.end() && _activeTabId == id) {
        if (_tabs.size() == 1) {
            // закрытие последней вкладки
            lastTabClosed.invoke();
            return;
        } else {
            auto nextIt = std::next(it);
            // переход на соседнюю от текущей вкладки
            if (nextIt != _tabsOrder.end()) {
                changeActiveTab(*(nextIt));
            } else if (nextIt != _tabsOrder.begin()) {
                changeActiveTab(*(std::prev(it)));
            }
        }
        _tabsOrder.erase(it);
        _tabs.erase(id);
        tabClosed.invoke(id);
    }
}

void TabManager::changeActiveTab(TabId id) {
    if (_tabs.find(id) != _tabs.end()) {
        _activeTabId = id;
        activeTabChanged.invoke(id);
    }
}

void TabManager::reloadTab(TabId id) {
    navigationRequested.invoke(
        NavigationRequestedArgs{NavigationType::Reload, getTab(id)->toTabInfo()});
}

void TabManager::visitUrl(TabId id, Url url) {
    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        existing->second->visitUrl(url);
    }
}

void TabManager::changeTabUrl(TabId id, Url url) {
    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        existing->second->changeUrl(url);
    }
}

void TabManager::changeTabTitle(TabId id, std::string title) {
    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        existing->second->changeTitle(title);
        titleChanged.invoke(TabTitleChangedArgs{id, title});
    }
}

void TabManager::changeTabLoadingProgress(TabId id, int progress) {
    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        existing->second->changeLoadingProgress(progress);
        loadingProgressChanged.invoke(TabLoadingProgressChangedArgs{id, progress});
    }
}

void TabManager::setTabLoadingStatus(TabId id, bool isLoading) {
    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        existing->second->setLoadingStatus(isLoading);
        loadingStatusChanged.invoke(TabLoadingStatusChangedArgs{id, isLoading});
    }
}

void TabManager::goBack(TabId id) {
    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        existing->second->goBack();
        navigationRequested.invoke(
            NavigationRequestedArgs{NavigationType::Back, getTab(id)->toTabInfo()});
    }
}

void TabManager::goForward(TabId id) {
    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        existing->second->goForward();
        navigationRequested.invoke(NavigationRequestedArgs{
                                                           NavigationType::Forward, getTab(id)->toTabInfo()});
    }
}

void TabManager::moveTab(TabId id, int newIndex) {
    auto it = std::find(_tabsOrder.begin(), _tabsOrder.end(), id);
    if (it != _tabsOrder.end()) {
        int oldIndex = it - _tabsOrder.begin();
        _tabsOrder.erase(it);
        _tabsOrder.insert(_tabsOrder.begin() + newIndex, id);
        tabMoved.invoke(TabMovedArgs{id, oldIndex, newIndex});
    }
}

Tab *TabManager::getTab(TabId id) {
    auto tabIt = _tabs.find(id);
    if (tabIt != _tabs.end()) {
        return tabIt->second.get();
    }
    return nullptr;
}

TabId TabManager::getActiveTabId() { return _activeTabId; }

const std::vector<TabId> &TabManager::getTabsOrder() { return _tabsOrder; }

const std::vector<Tab> TabManager::getTabs() {
    // return std::vector<Tab>(_tabs.begin(), _tabs.end());
    return {};
}

const std::vector<TabInfo> TabManager::getTabInfos() {
    std::vector<TabInfo> result;
    result.reserve(_tabs.size());
    // конвертация map<tabId, tab> в vector<tabInfo>
    std::transform(_tabs.begin(), _tabs.end(),
                   std::back_inserter(
                       result), // back_inserter - для вставки в конец контейнера
                   [](const auto &pair) // передача без копирования
                   { return pair.second->toTabInfo(); });
    return result;
}

bool TabManager::canGoBack(TabId id) {
    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        return existing->second->canGoBack();
    }
    return false;
}
bool TabManager::canGoForward(TabId id) {

    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        return existing->second->canGoForward();
    }
    return false;
}
