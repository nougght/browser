#ifndef TABS_CONTROLLER_H
#define TABS_CONTROLLER_H

#include <QObject>
#include <core/Identifier.h>
#include "../adapter/CoreAdapter.h"
#include "../ui/mainwindow.h"

class TabsController : public QObject
{
    Q_OBJECT

public:
    TabsController(CoreAdapter *coreAdapter, TabsModel *tabsModel);

    // slots for core signals
    void onTabsLoaded(std::vector<TabInfo>);
    void onTabCreated(TabInfo tabInfo);
    void onActiveTabChanged(TabId id);
    void onNavigationRequested(NavigationRequestedArgs args);
    void onTabTitleChanged(TabTitleChangedArgs args);
    void onLoadingStatusChanged(TabLoadingStatusChangedArgs args);
    void onLoadingProgressChanged(TabLoadingProgressChangedArgs args);



    void onTabsModelDataChanged(const QModelIndex &topLeft,
                                const QModelIndex &bottomRight,
                                const QList<int> &roles = QList<int>());


    // slots for ui signals
    void onSearchRequested(QString searchQuery);
    void onEngineUrlChanged(TabId id, QUrl newUrl);
    void onEngineTitleChanged(TabId id, QString newTitle);
    void onLoadStarted(TabId id);
    void onLoadFinished(TabId id);
    void onLoadProgress(TabId id, int progress);
    void onNewTabClicked();
    void onTabClicked(int index);
    void onReloadClicked();
    void onBackClicked();
    void onForwardClicked();

signals:
    void urlVisitRequested(TabInfo tab);
    void backNavigationRequested(TabInfo tab);
    void forwardNavigationRequested(TabInfo tab);
    void reloadRequested(TabId tabId);

    void activeTabUrlChanged(QUrl newUrl);
    void backNavigationAvailabilityChanged(bool canGoBack);
    void forwardNavigationAvailabilityChanged(bool canGoForward);

    void tabsLoaded(std::vector<TabInfo> tabs);
    void tabCreated(TabInfo tabInfo);
    void activeTabChanged(TabInfo tabInfo);
    void activeTabLoadingStatusChanged(bool isLoading);
    void activeTabLoadingProgressChanged(int progress);


private:

    void _setupEvents();

    CoreAdapter *_coreAdapter;
    TabsModel *_tabsModel;
    TabId _activeTabId;
};

#endif
