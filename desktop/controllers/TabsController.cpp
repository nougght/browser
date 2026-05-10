#include "TabsController.h"

TabsController::TabsController(CoreAdapter *coreAdapter, TabsModel *tabsModel)
    : _coreAdapter(coreAdapter), _tabsModel(tabsModel)
{
    _setupEvents();
}

void TabsController::_setupEvents()
{
    // connect(_mainWindow, &MainWindow::newTabClicked, this, &TabsController::onNewTabClicked);
    // connect(_mainWindow, &MainWindow::tabClicked, this, &TabsController::onTabClicked);
    // connect(_mainWindow, &MainWindow::reloadClicked, this, &TabsController::onReloadClicked);
    // connect(_mainWindow, &MainWindow::backClicked, this, &TabsController::onBackClicked);
    // connect(_mainWindow, &MainWindow::forwardClicked, this, &TabsController::onForwardClicked);
    // connect(_mainWindow, &MainWindow::searchClicked, this, &TabsController::onSearchEditingClicked);
    // connect(_mainWindow, &MainWindow::engineUrlChanged, this, &TabsController::onEngineUrlChanged);
    // connect(_mainWindow, &MainWindow::engineTitleChanged, this, &TabsController::onEngineTitleChanged);
    // connect(_mainWindow, &MainWindow::loadStarted, this, [this] (TabId id){_core->setTabLoadingStatus(id, true);});
    // connect(_mainWindow, &MainWindow::loadFinished, this, [this] (TabId id){_core->setTabLoadingStatus(id, false);});
    // connect(_mainWindow, &MainWindow::loadProgress, this, [this] (TabId id, int progress){_core->changeTabLoadingProgress(id, progress);});

    // connect(mainWindow, &MainWindow::newTabClicked, this, &TabsController::onNewTabClicked);

    connect(_coreAdapter, &CoreAdapter::tabsLoaded, this, &TabsController::onTabsLoaded);
    connect(_coreAdapter, &CoreAdapter::tabCreated, this, &TabsController::onTabCreated);
    connect(_coreAdapter, &CoreAdapter::activeTabChanged, this, &TabsController::onActiveTabChanged);
    connect(_coreAdapter, &CoreAdapter::loadingStatusChanged, this, &TabsController::onLoadingStatusChanged);
    connect(_coreAdapter, &CoreAdapter::loadingProgressChanged, this, &TabsController::onLoadingProgressChanged);
    connect(_coreAdapter, &CoreAdapter::titleChanged, this, &TabsController::onTabTitleChanged);
    connect(_coreAdapter, &CoreAdapter::navigationRequested, this, &TabsController::onNavigationRequested);

    connect(_tabsModel, &TabsModel::dataChanged, this, &TabsController::onTabsModelDataChanged);
    // connect(_coreAdapter, &CoreAdapter::tabsLoaded, this, &TabsController::onTabsLoaded);
}


// slots for mainwindow signals

void TabsController::onNewTabClicked()
{
    qDebug() << "\nNew tab clicked";
    _coreAdapter->createTab();
}

void TabsController::onTabClicked(int index)
{
    qDebug() << "\nTab with index " << index << " clicked";
    auto tabId = _tabsModel->getTabIdByIndex(index);
    _coreAdapter->changeActiveTab(tabId);
}


void TabsController::onReloadClicked()
{
    qDebug() << "\nReloading tab " << _activeTabId.value;
    _coreAdapter->reloadTab(_activeTabId);
}

void TabsController::onBackClicked()
{
    qDebug() << "\nGoing back in tab " << _activeTabId.value;
    _coreAdapter->goBack(_activeTabId);
}

void TabsController::onForwardClicked()
{
    qDebug() << "\nGoing forward in tab " << _activeTabId.value;
    _coreAdapter->goForward(_activeTabId);
}

void TabsController::onSearchRequested(QString searchQuery)
{
    // goToWebsite(_search->text());
    qDebug() << "\nSearch requested: " << searchQuery;
    // qDebug() << "\nSIZE = " << _core->urlVisited.handlersSize();
    _coreAdapter->visitUrl(_activeTabId, searchQuery);
}

// redirect
void TabsController::onEngineUrlChanged(TabId id, QUrl newUrl)
{
    qDebug() << "\nEngine url changed: " << newUrl.toString() << " for tab " << id.value;
    // QMetaObject::invokeMethod(this, [this, newUrl]{
    _coreAdapter->changeTabUrl(id, newUrl);
    // }, Qt::QueuedConnection);
}

void TabsController::onEngineTitleChanged(TabId id, QString newTitle)
{
    qDebug() << "\nEngine title changed: " << newTitle << " for tab " << id.value;
    _coreAdapter->changeTabTitle(id, newTitle);
}

void TabsController::onLoadStarted(TabId id)
{
    qDebug() << "\nLoading started for tab " << id.value;
    _coreAdapter->setTabLoadingStatus(id, true);
}

void TabsController::onLoadFinished(TabId id)
{
    qDebug() << "\nLoading finished for tab " << id.value;
    _coreAdapter->setTabLoadingStatus(id, false);
}

void TabsController::onLoadProgress(TabId id, int progress)
{
    qDebug() << "\nLoading progress for tab " << id.value << ": " << progress << "%";
    _coreAdapter->changeTabLoadingProgress(id, progress);
}




// slots for core signals

void TabsController::onTabsLoaded(std::vector<TabInfo> tabs)
{
    auto id = tabs.back().id;
    _tabsModel->setInitialTabs(tabs);
    emit tabsLoaded(std::move(tabs));
    
    _coreAdapter->changeActiveTab(id);
}

void TabsController::onTabCreated(TabInfo tabInfo)
{
    _tabsModel->addTab(tabInfo);
    _activeTabId = tabInfo.id;
    emit tabCreated(tabInfo);
}

void TabsController::onActiveTabChanged(TabId id)
{
    auto tabInfo = _tabsModel->getTabInfo(id);
    _activeTabId = id;
    qDebug() << "\nActive tab changed: " << id.value << " url: " << QString::fromStdString(tabInfo.url.toStdString());
    emit activeTabChanged(tabInfo);
}

void TabsController::onTabTitleChanged(TabTitleChangedArgs args)
{
    _tabsModel->updateTabTitle(args.id, args.newTitle);
}

void TabsController::onLoadingStatusChanged(TabLoadingStatusChangedArgs args)
{
    _tabsModel->updateTabLoading(args.id, args.isLoading);
}

void TabsController::onLoadingProgressChanged(TabLoadingProgressChangedArgs args)
{
    // tabmodel doesn't store loading progress
    if (args.id == _activeTabId)
    {
        emit activeTabLoadingProgressChanged(args.progress);
    }
}

// sending update signals to mainwindow
void TabsController::onNavigationRequested(NavigationRequestedArgs args)
{
    qDebug() << "\nNavigation requested for tab " << args.tabInfo.id.value << " with url " << QString::fromStdString(args.tabInfo.url.toStdString()) << " and navigation type " << static_cast<int>(args.type);
    switch (args.type)
    {
    case NavigationType::NewPage:
        _tabsModel->updateTabUrl(args.tabInfo.id, args.tabInfo.url);
        _tabsModel->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
        emit urlVisitRequested(args.tabInfo);
        break;
    case NavigationType::Back:
        _tabsModel->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
        emit backNavigationRequested(args.tabInfo);
        break;
    case NavigationType::Forward:
        _tabsModel->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
        emit forwardNavigationRequested(args.tabInfo);
        break;

    case NavigationType::Redirect:
        _tabsModel->updateTabUrl(args.tabInfo.id, args.tabInfo.url);
        _tabsModel->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
        // redirected(CoreAdapter::convert(args.tabInfo.url));
        // _tabBar->updateTabNavigation(_activeTabId, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
        break;
    case NavigationType::Reload:
        // onUrlVisited(convert(args.tabInfo.url));
        reloadRequested(args.tabInfo.id);
        break;
    }
}

void TabsController::onTabsModelDataChanged(const QModelIndex &topLeft,
                                            const QModelIndex &bottomRight,
                                            const QList<int> &roles)
{
    if (topLeft.row() != _tabsModel->getTabIndex(_activeTabId))
    {
        return;
    }
    if (roles.contains(TabsModel::Roles::UrlRole))
    {
        emit activeTabUrlChanged(_tabsModel->index(topLeft.row(), 0).data(TabsModel::UrlRole).toUrl());
    }
    if (roles.contains(TabsModel::Roles::IsLoadingRole))
    {
        emit activeTabLoadingStatusChanged(_tabsModel->index(topLeft.row(), 0).data(TabsModel::IsLoadingRole).toBool());
    }

    if (roles.contains(TabsModel::Roles::BackNavigatingRole))
    {
        emit backNavigationAvailabilityChanged(_tabsModel->index(topLeft.row(), 0).data(TabsModel::BackNavigatingRole).toBool());
    }

    if (roles.contains(TabsModel::Roles::ForwardNavigatingRole))
    {
        emit forwardNavigationAvailabilityChanged(_tabsModel->index(topLeft.row(), 0).data(TabsModel::ForwardNavigatingRole).toBool());
    }
}
