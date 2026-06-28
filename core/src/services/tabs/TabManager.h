#ifndef TAB_MANAGER_H
#define TAB_MANAGER_H

// #include <core/Identifier.h>
#include "domain/Tab.h"
#include "TabIdGenerator.h"
#include <unordered_map>
#include <vector>
#include <memory>

class TabManager
{
private:
    std::unordered_map<TabId, std::unique_ptr<Tab>> _tabs;
    // порядок вкладок
    std::vector<TabId> _tabsOrder;
    TabIdGenerator _idGenerator;
    TabId _activeTabId;

    SearchSettings _searchSettings;

    std::pair<TabId, std::unique_ptr<Tab>> _findTab(TabId id);

    Url _newTabUrl();

    Url _buildSearchUrl(std::string query);
    std::optional<Url> _resolveQuery(std::string query);

    
    void _visitUrl(TabId id, Url url);

public:
    TabManager();
    void loadTabs();

    void setSearchEngine(SearchEngine engine);
    void handleSearchQuery(TabId id, std::string query);
    void openInternalPage(InternalPageType type, bool isNewTab = true);

    TabId createTab(Url url, bool isBackground);
    TabId createTab(bool isBackground);
    void closeTab(TabId id);
    TabId getActiveTabId();
    void changeActiveTab(TabId id);
    void moveTab(TabId id, int newIndex);
    Tab *getTab(TabId id);
    const std::vector<TabId> &getTabsOrder();
    const std::vector<Tab> getTabs();
    const std::vector<TabInfo> getTabInfos();

    bool canGoBack(TabId id);
    bool canGoForward(TabId id);
    void goBack(TabId id);
    void goForward(TabId id);

    void onNavigationRequested(NavigationType type, TabId id, Url url);
    void onEngineUrlChanged(TabId id, Url url);
    void onEngineTitleChanged(TabId id, std::string title);
    
    void changeTabLoadingProgress(TabId id, int progress);
    void setTabLoadingStatus(TabId id, bool isLoading);
    void reloadTab(TabId id);


    Event<std::vector<TabInfo>> tabsLoaded;

    // создана новая вкладка
    Event<TabInfo> tabCreated;

    // переходы в рамках одной вкладки
    Event<NavigationCommandArgs> navigationCommand;
    Event<NavigationCompletedArgs> navigationCompleted;

    Event<TabTitleChangedArgs> titleChanged;

    Event<TabLoadingProgressChangedArgs> loadingProgressChanged;

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

    // Event<TabId> tabReloaded;
};

#endif
