#ifndef MAIN_WINDOW_CONTROLLER_H
#define MAIN_WINDOW_CONTROLLER_H

#include <QObject>
#include <core/Identifier.h>
#include "adapter/CoreAdapter.h"
#include "ui/mainwindow.h"

// TODO: check model signals usage

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(CoreAdapter *coreAdapter);
    void onNavigationRequested(NavigationRequestedArgs args);
    void onTabsLoaded(std::vector<TabInfo>);
    void onTabCreated(TabInfo tabInfo);
    void onSearchEditingFinished();
    void onEngineUrlChanged(TabId id, QUrl newUrl);
    void onEngineTitleChanged(TabId id, QString newTitle);
    void onLoadingStatusChanged(TabId id, bool isLoading);
    void onLoadingProgressChanged(TabId id, int progress);
    void onActiveTabChanged(TabId id);


    void onTabsModelDataChanged(const QModelIndex &topLeft,
                                const QModelIndex &bottomRight,
                                const QList<int> &roles = QList<int>());

signals:
    void urlVisitRequested(TabInfo tab);
    void backNavigationRequested(TabInfo tab);
    void forwardNavigationRequested(TabInfo tab);
    void reloadRequested(TabId tabId);

    void activeTabUrlChanged(TabId id, QUrl newUrl);
    void backNavigationAvailabilityChanged(bool canGoBack);
    void forwardNavigationAvailabilityChanged(bool canGoForward);


    void tabsLoaded(std::vector<TabInfo> tabs);
    void tabCreated(TabInfo tabInfo);
    void activeTabChanged(TabInfo tabInfo);
    void activeTabLoadingStatusChanged(bool isLoading);
    void activeTabLoadingProgressChanged(int progress);

private:
    void _setupEvents();

    // MainWindow *_mainWindow;
    CoreAdapter *_coreAdapter;

    TabsModel *_tabsModel;
    TabId _activeTabId;
};

#endif
