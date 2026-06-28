#ifndef TABS_CONTROLLER_H
#define TABS_CONTROLLER_H

#include <QObject>
#include <core/Identifier.h>
#include "adapter/CoreAdapter.h"
#include "models/UIContext.h"

class TabsController : public QObject
{
    Q_OBJECT

public:
    TabsController(CoreAdapter *coreAdapter, UIContext *ctx);
    // slots for core signals
    void onTabsLoaded(std::vector<TabInfo>);
    void onTabCreated(TabInfo tabInfo);
    void onTabClosed(TabId id);
    void onLastTabClosed();
    void onActiveTabChanged(TabId id);

    void onNavigationCommand(NavigationCommandArgs args);
    void onNavigationCompleted(NavigationCompletedArgs args);

    void onTabTitleChanged(TabTitleChangedArgs args);
    void onLoadingStatusChanged(TabLoadingStatusChangedArgs args);
    void onLoadingProgressChanged(TabLoadingProgressChangedArgs args);



    void onTabsModelDataChanged(const QModelIndex &topLeft,
                                const QModelIndex &bottomRight,
                                const QList<int> &roles = QList<int>());


    // slots for ui signals
    void onSearchRequested(QString searchQuery);

    void onNavigationRequested(NavigationType type, TabId id, Url url);
    void onEngineUrlChanged(TabId id, QUrl newUrl);
    void onEngineTitleChanged(TabId id, QString newTitle);
    void onLoadStarted(TabId id);
    void onLoadFinished(TabId id);
    void onLoadProgress(TabId id, int progress);
    void onNewTabClicked();
    void onTabClicked(int index);
    void onCloseTabClicked(int index);
    void onReloadClicked();
    void onBackClicked();
    void onForwardClicked();

signals:
    // navigation commands
    void urlVisitCommand(TabId tabId, Url url);
    void backNavigationCommand(TabId tabId);
    void forwardNavigationCommand(TabId tabId);
    void reloadCommand(TabId tabId);

    // navigation completed notification
    void navigationCompleted(NavigationCompletedArgs args, bool isBookmarked);
    

    void activeTabUrlChanged(Url newUrl);
    void backNavigationAvailabilityChanged(bool canGoBack);
    void forwardNavigationAvailabilityChanged(bool canGoForward);

    void tabsLoaded(std::vector<TabInfo> tabs);
    void tabCreated(TabInfo tabInfo);
    void tabClosed(TabId id);
    void lastTabClosed();
    void activeTabChanged(TabInfo tabInfo, bool isBookmarked);
    void activeTabLoadingStatusChanged(bool isLoading);
    void activeTabLoadingProgressChanged(int progress);


private:
    void _setupEvents();

    CoreAdapter *_coreAdapter;
    UIContext *_ctx;
};

#endif
