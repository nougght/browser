#include "MainWindowController.h"


MainWindowController::MainWindowController(CoreAdapter * coreAdapter)
    : _coreAdapter(coreAdapter)
{

}

void MainWindowController::_setupEvents()
{
    // connect(_coreAdapter, &CoreAdapter::tabsLoaded, this, &MainWindowController::onTabsLoaded);
    // connect(_coreAdapter, &CoreAdapter::tabCreated, this, &MainWindowController::onTabCreated);
    // connect(_coreAdapter, &CoreAdapter::activeTabChanged, &MainWindowController::onActiveTabChanged);
    // connect(_coreAdapter, &CoreAdapter::titleChanged, this, &MainWindowController::onEngineTitleChanged);
    // connect(_coreAdapter, &CoreAdapter::loadingStatusChanged, this, &MainWindowController::onLoadingStatusChanged);
    // connect(_coreAdapter, &CoreAdapter::loadingProgressChanged, this, &MainWindowController::onLoadingProgressChanged);
    // connect(_coreAdapter, &CoreAdapter::navigationRequested, this, &MainWindowController::onNavigationRequested);
    
}

// void MainWindowController::onTabsLoaded(std::vector<TabInfo> tabs)
// {
//     _tabsModel->setInitialTabs(tabs);
//     emit tabsLoaded(std::move(tabs));
// }

// void MainWindowController::onTabCreated(TabInfo tabInfo)
// {
//     _tabsModel->addTab(tabInfo);
//     emit tabCreated(tabInfo);
// }

// void MainWindowController::onActiveTabChanged(TabId id)
// {
//     auto tabInfo = _tabsModel->getTabInfo(id);
//     emit activeTabChanged(tabInfo);
// }

// void MainWindowController::onEngineTitleChanged(TabId id, QString newTitle)
// {
//     _tabsModel->updateTabTitle(id, newTitle.toStdString());
// }

// void MainWindowController::onLoadingStatusChanged(TabId id, bool isLoading)
// {
//     _tabsModel->updateTabLoading(id, isLoading);
// }


// void MainWindowController::onLoadingProgressChanged(TabId id, int progress)
// {
//     // tabmodel doesn't store loading progress
//     if (id == _activeTabId)
//     {
//         emit activeTabLoadingProgressChanged(progress);
//     }
// }

// // sending update signals to mainwindow
// void MainWindowController::onNavigationRequested(NavigationRequestedArgs args)
// {
//     // qDebug() << "\nNavigationHandlers " << _core->navigationCompleted.handlersSize() << "\n";
//     switch (args.type)
//     {
//     case NavigationType::NewPage:
//         _tabsModel->updateTabUrl(args.tabInfo.id, args.tabInfo.url);
//         _tabsModel->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
//         emit urlVisitRequested(args.tabInfo);
//         break;
//     case NavigationType::Back:
//         _tabsModel->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
//         emit backNavigationRequested(args.tabInfo);
//         break;
//     case NavigationType::Forward:
//         _tabsModel->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
//         emit forwardNavigationRequested(args.tabInfo);
//         break;

//     case NavigationType::Redirect:
//         _tabsModel->updateTabUrl(args.tabInfo.id, args.tabInfo.url);
//         _tabsModel->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
//         // redirected(CoreAdapter::convert(args.tabInfo.url));
//         // _tabBar->updateTabNavigation(_activeTabId, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
//         break;
//     case NavigationType::Reload:
//         // onUrlVisited(convert(args.tabInfo.url));
//         reloadRequested(args.tabInfo.id);
//         break;
//     }
// }


// void MainWindowController::onTabsModelDataChanged(const QModelIndex &topLeft,
//                                 const QModelIndex &bottomRight,
//                                 const QList<int> &roles)
// {
//     if (topLeft.row() != _tabsModel->getTabIndex(_activeTabId))
//     {
//         return;
//     }
//     if (roles.contains(TabsModel::Roles::UrlRole))
//     {
//         emit activeTabUrlChanged(_activeTabId, _tabsModel->index(topLeft.row(), 0).data(TabsModel::UrlRole).toUrl());
//     }
//     if (roles.contains(TabsModel::Roles::IsLoadingRole))
//     {
//         emit activeTabLoadingStatusChanged(_tabsModel->index(topLeft.row(), 0).data(TabsModel::IsLoadingRole).toBool());
//     }

//     if (roles.contains(TabsModel::Roles::BackNavigatingRole))
//     {
//         emit backNavigationAvailabilityChanged(_tabsModel->index(topLeft.row(), 0).data(TabsModel::BackNavigatingRole).toBool());
//     }

//     if (roles.contains(TabsModel::Roles::ForwardNavigatingRole))
//     {
//         emit forwardNavigationAvailabilityChanged(_tabsModel->index(topLeft.row(), 0).data(TabsModel::ForwardNavigatingRole).toBool());
//     }
// }
