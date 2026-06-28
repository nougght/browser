#include "TabManager.h"
#include "core/Url.h"
#include "core/eventArgs.h"
#include <algorithm>
#include <iterator>

#include <util/util.h>
#include <ada.h>

TabManager::TabManager() {
    setSearchEngine(SearchEngine::Google);
    searchEngineLoaded.invoke(SearchEngine::Google);
}

// url of new tab with search engine base url
Url TabManager::_newTabUrl(){
    return Url::newTabUrl(_searchSettings.baseUrl);
}


void TabManager::setSearchEngine(SearchEngine engine) {
    _searchSettings.engine = engine;
    _searchSettings.baseUrl = searchEngineToBaseUrl(engine);
    _searchSettings.searchPath = searchEngineToSearchPath(engine);
    _searchSettings.searchQueryParam = searchEngineToSearchQueryParam(engine);
}

// TODO: session restore
void TabManager::loadTabs() {
    openInternalPage(InternalPageType::NewTab, true);
}

// create new tab with optional url
TabId TabManager::createTab(Url url, bool isBackground) {
    auto tab = std::make_unique<Tab>(_idGenerator.create());
    TabId id = tab->getId();
    _tabs.emplace(id, std::move(tab));
    _tabsOrder.push_back(id);

    tabCreated.invoke(getTab(id)->toTabInfo());
    
    if (!isBackground) {
        changeActiveTab(id);
    }

    if (!url.isEmpty() && !url.isInternal()) {
        _visitUrl(id, url);
    } 
    return id;
}

TabId TabManager::createTab(bool isBackground) {
    return createTab(_newTabUrl(), isBackground);
}

void TabManager::closeTab(TabId id) {
    auto it = std::find(_tabsOrder.begin(), _tabsOrder.end(), id);

    if (!id.isValid() || it == _tabsOrder.end()) {
        std::printf("TM closeTab tab not found\n");
        return;
    }
    if (_tabs.size() == 1) {
        std::printf("TM closeTab last tab\n");
        // закрытие последней вкладки
        lastTabClosed.invoke();
        return;
    } else if (id == _activeTabId) {
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
    std::printf("TM closeTab tab closed\n");
    tabClosed.invoke(id);
    
}


void TabManager::_visitUrl(TabId id, Url url) {
    auto tab = getTab(id);
    if (tab == nullptr) {
        std::printf("TM visitUrl not found\n");
        return;
    }
    std::printf("TM visitUrl %s\n", url.toStdString().c_str());
    navigationCommand.invoke(NavigationCommandArgs{
                                                       NavigationType::NewPage, id, url});
}

void TabManager::onNavigationRequested(NavigationType type, TabId id, Url url) {
    switch (type) {
        case NavigationType::NewPage: {
            auto tab = getTab(id);
            if (tab == nullptr) {
                std::printf("TM onNavigationCommand tab not found\n");
                return;
            }
            // не меняем url новой вкладки
            if (tab->getInternalPageType().has_value()) {
                switch (tab->getInternalPageType().value()) {
                    case InternalPageType::NewTab:
                        return;
                    default:
                        break;
                }
            }
            tab->visitUrl(url);
            break;
        }
        case NavigationType::Reload: {
            break;
        }
        case NavigationType::Redirect: {
            auto tab = getTab(id);
            if (tab == nullptr) {
                std::printf("TM onNavigationCommand tab not found\n");
                return;
            }
            tab->changeUrl(url);
        }
    }

    navigationCompleted.invoke(NavigationCompletedArgs{type, getTab(id)->toTabInfo()});
}

void TabManager::changeActiveTab(TabId id) {
    if (_tabs.find(id) != _tabs.end()) {
        _activeTabId = id;
        activeTabChanged.invoke(id);
    }
}

void TabManager::reloadTab(TabId id) {
    navigationCommand.invoke(
        NavigationCommandArgs{NavigationType::Reload, id, Url()});
}

Url TabManager::_buildSearchUrl(std::string query) {
    auto adaUrl = ada::parse(_searchSettings.baseUrl);
    if (!adaUrl) {
        throw std::runtime_error("TM buildSearchUrl ada::parse failed");
    }
    ada::url_search_params params;
    params.append(_searchSettings.searchQueryParam, query);
    adaUrl->set_pathname(_searchSettings.searchPath);
    adaUrl->set_search(params.to_string());
    auto href = adaUrl->get_href();
    auto optionalUrl = Url::parse(std::string(href.begin(), href.end()));
    if (!optionalUrl.has_value()) {
        throw std::runtime_error("TM buildSearchUrl Url::parse failed");
    }
    return optionalUrl.value();
}

std::optional<Url> TabManager::_resolveQuery(std::string query) {
    query = trimSpaces(query);
    if (query.empty()) {
        return _newTabUrl();
    }

    if (query.find("br://") == 0) {
        if (query == NewTabUrl) {
            return _newTabUrl();
        }
        return Url::parse(query);
    }

    if (isLikeUrl(query)) {
        std::printf("TM resolveQuery like url\n");
        if (auto i = query.find("://"); i != std::string::npos) {
            auto protocol = query.substr(0, i);
            // if (protocol != "http" && protocol != "https" && protocol != "br") {
            //     std::printf("TM resolveQuery invalid protocol\n");
            //     return std::nullopt;
            // }
            return Url::parse(query);
        } else {
            query = "https://" + query;
            return Url::parse(query);
        }
    } else {
        return _buildSearchUrl(query);
    }

    return std::nullopt;
}

void TabManager::handleSearchQuery(TabId id, std::string query) {
    auto existing = _tabs.find(id);
    if (existing == _tabs.end()) {
        std::printf("TM visitUrl not found\n");
        return;
    }
    auto optionalUrl = _resolveQuery(query);
    if (!optionalUrl.has_value()) {
        std::printf("TM resolveQuery failed\n");
        return;
    }
    _visitUrl(id, optionalUrl.value());
}

void TabManager::openInternalPage(InternalPageType type, bool isNewTab) {
    auto newTabUrl = Url::parse(_searchSettings.baseUrl);
    if (!newTabUrl.has_value()) {
        std::printf("TM openInternalPage Url::parse failed\n");
        return;
    }
    auto url = getInternalPageTypeUrl(type, type == InternalPageType::NewTab ? newTabUrl.value() : Url()); 
    if (!url.has_value()) {
        std::printf("TM openInternalPage url not found\n");
        return;
    }
    TabId id;
    if (isNewTab) {
        id = createTab(url.value(), false);
    } else {
        id = _activeTabId;
    }

    auto tab = getTab(id);
    if (tab == nullptr) {
        std::printf("TM openInternalPage tab not found\n");
        return;
    }
    tab->visitUrl(url.value());
    _visitUrl(id, url.value());
    if (auto type = tab->getInternalPageType(); type.has_value()) {
        tab->changeTitle(getInternalPageTypeTitle(type.value()));
    }
    navigationCompleted.invoke(NavigationCompletedArgs{NavigationType::NewPage, 
        getTab(id)->toTabInfo(),
        });
}

// void TabManager::onEngineUrlChanged(TabId id, Url url) {
//     _changeTabUrl(id, url);
//     navigationRequested.invoke(
//         NavigationRequestedArgs{NavigationType::Redirect, getTab(id)->toTabInfo()});
// }

// void TabManager::onEngineTitleChanged(TabId id, std::string title) {
//     _changeTabTitle(id, title);
//     titleChanged.invoke(TabTitleChangedArgs{id, title});
// }

void TabManager::onEngineUrlChanged(TabId id, Url url) {
    Tab * tab = getTab(id);
    if (tab == nullptr) {
        std::printf("TM changeTabUrl tab not found\n");
        return;
    }
    // если url тот же
    if (tab->getUrl() == url) {
        std::printf("TM changeTabUrl url same\n");
        return;
    } 
    std::printf("TM changeTabUrl not same");
    if (tab->getInternalPageType().has_value()) {
        switch (tab->getInternalPageType().value()) {
            case InternalPageType::NewTab:
                return;
            default:
                break;
        }
    }

    tab->changeUrl(url);
    navigationCompleted.invoke(NavigationCompletedArgs{NavigationType::Redirect, getTab(id)->toTabInfo()});
}

void TabManager::onEngineTitleChanged(TabId id, std::string title) {
    std::printf("TM changeTabTitle");
    auto existing = _tabs.find(id);
    if (existing == _tabs.end()) {
        std::printf("TM changeTabTitle not found\n");
        return;
    }
    auto tab = existing->second.get();
    if (tab->getInternalPageType().has_value()) {
        switch (tab->getInternalPageType().value()) {
            case InternalPageType::NewTab:
                return;
            default:
                break;
        }
    }
    existing->second->changeTitle(title);
    titleChanged.invoke(TabTitleChangedArgs{id, title});
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
        navigationCommand.invoke(
            NavigationCommandArgs{NavigationType::Back, id, Url()});
    }
}

void TabManager::goForward(TabId id) {
    auto existing = _tabs.find(id);
    if (existing != _tabs.end()) {
        existing->second->goForward();
        navigationCommand.invoke(NavigationCommandArgs{NavigationType::Forward, id, Url()});
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
