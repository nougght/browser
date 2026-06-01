#include "TabsController.h"

TabsController::TabsController(CoreAdapter *coreAdapter, UIContext *ctx)
    : _coreAdapter(coreAdapter), _ctx(ctx)
{
    _setupEvents();
}

void TabsController::_setupEvents()
{

    connect(_coreAdapter, &CoreAdapter::tabsLoaded, this, &TabsController::onTabsLoaded);
    connect(_coreAdapter, &CoreAdapter::tabCreated, this, &TabsController::onTabCreated);
    connect(_coreAdapter, &CoreAdapter::tabClosed, this, &TabsController::onTabClosed);
    connect(_coreAdapter, &CoreAdapter::lastTabClosed, this, &TabsController::lastTabClosed);
    connect(_coreAdapter, &CoreAdapter::activeTabChanged, this, &TabsController::onActiveTabChanged);
    connect(_coreAdapter, &CoreAdapter::loadingStatusChanged, this, &TabsController::onLoadingStatusChanged);
    connect(_coreAdapter, &CoreAdapter::loadingProgressChanged, this, &TabsController::onLoadingProgressChanged);
    connect(_coreAdapter, &CoreAdapter::titleChanged, this, &TabsController::onTabTitleChanged);
    connect(_coreAdapter, &CoreAdapter::navigationRequested, this, &TabsController::onNavigationRequested);

    connect(_ctx->getTabsModel(), &TabsModel::dataChanged, this, &TabsController::onTabsModelDataChanged);
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
    auto tabId = _ctx->getTabsModel()->getTabIdByIndex(index);
    _coreAdapter->changeActiveTab(tabId);
}

void TabsController::onCloseTabClicked(int index)
{
    qDebug() << "\nTab with index " << index << " close clicked";
    auto tabId = _ctx->getTabsModel()->getTabIdByIndex(index);
    _coreAdapter->closeTab(tabId);
}

void TabsController::onReloadClicked()
{
    qDebug() << "\nReloading tab " << _ctx->activeTabId().value;
    _coreAdapter->reloadTab(_ctx->activeTabId());
}

void TabsController::onBackClicked()
{
    qDebug() << "\nGoing back in tab " << _ctx->activeTabId().value;
    _coreAdapter->goBack(_ctx->activeTabId());
}

void TabsController::onForwardClicked()
{
    qDebug() << "\nGoing forward in tab " << _ctx->activeTabId().value;
    _coreAdapter->goForward(_ctx->activeTabId());
}

void TabsController::onSearchRequested(QString searchQuery)
{
    // goToWebsite(_search->text());
    qDebug() << "\nSearch requested: " << searchQuery;
    // qDebug() << "\nSIZE = " << _core->urlVisited.handlersSize();
    _coreAdapter->visitUrl(_ctx->activeTabId(), searchQuery);
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
    _ctx->getTabsModel()->setInitialTabs(tabs);
    emit tabsLoaded(std::move(tabs));
}

void TabsController::onTabCreated(TabInfo tabInfo)
{
    _ctx->getTabsModel()->addTab(tabInfo);
    emit tabCreated(tabInfo);
}


void TabsController::onTabClosed(TabId id)
{
    _ctx->getTabsModel()->removeTab(id);
    emit tabClosed(id);
}

void TabsController::onLastTabClosed()
{
    emit lastTabClosed();
}

void TabsController::onActiveTabChanged(TabId id)
{
    auto tabInfo = _ctx->getTabsModel()->getTabInfo(id);
    _ctx->setActiveTabId(id);
    qDebug() << "\nActive tab changed: " << id.value << " url: " << QString::fromStdString(tabInfo.url.toStdString());
    bool isBookmarked = _ctx->getBookmarkModel()->isBookmarked(tabInfo.url.toStdString());
    emit activeTabChanged(tabInfo, isBookmarked);
}

void TabsController::onTabTitleChanged(TabTitleChangedArgs args)
{
    _ctx->getTabsModel()->updateTabTitle(args.id, args.newTitle);
}

void TabsController::onLoadingStatusChanged(TabLoadingStatusChangedArgs args)
{
    _ctx->getTabsModel()->updateTabLoading(args.id, args.isLoading);
}

void TabsController::onLoadingProgressChanged(TabLoadingProgressChangedArgs args)
{
    // tabmodel doesn't store loading progress
    if (args.id == _ctx->activeTabId())
    {
        emit activeTabLoadingProgressChanged(args.progress);
    }
}

// sending update signals to mainwindow
void TabsController::onNavigationRequested(NavigationRequestedArgs args)
{
    qDebug() << "\nNavigation requested for tab " << args.tabInfo.id.value << " with url " << QString::fromStdString(args.tabInfo.url.toStdString()) << " and navigation type " << static_cast<int>(args.type);
    bool isBookmarked = _ctx->getBookmarkModel()->isBookmarked(args.tabInfo.url.toStdString());
    switch (args.type)
    {
    case NavigationType::NewPage:
        _ctx->getTabsModel()->updateTabUrl(args.tabInfo.id, args.tabInfo.url);
        _ctx->getTabsModel()->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
        emit urlVisitRequested(args.tabInfo, isBookmarked);
        break;
    case NavigationType::Back:
        _ctx->getTabsModel()->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
        emit backNavigationRequested(args.tabInfo, isBookmarked);
        break;
    case NavigationType::Forward:
        _ctx->getTabsModel()->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
        emit forwardNavigationRequested(args.tabInfo, isBookmarked);
        break;

    case NavigationType::Redirect:
        _ctx->getTabsModel()->updateTabUrl(args.tabInfo.id, args.tabInfo.url);
        _ctx->getTabsModel()->updateTabNavigation(args.tabInfo.id, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
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
    if (topLeft.row() != _ctx->getTabsModel()->getTabIndex(_ctx->activeTabId()))
    {
        return;
    }
    if (roles.contains(TabsModel::Roles::UrlRole))
    {
        emit activeTabUrlChanged(_ctx->getTabsModel()->index(topLeft.row(), 0).data(TabsModel::UrlRole).toUrl());
    }
    if (roles.contains(TabsModel::Roles::IsLoadingRole))
    {
        emit activeTabLoadingStatusChanged(_ctx->getTabsModel()->index(topLeft.row(), 0).data(TabsModel::IsLoadingRole).toBool());
    }

    if (roles.contains(TabsModel::Roles::BackNavigatingRole))
    {
        emit backNavigationAvailabilityChanged(_ctx->getTabsModel()->index(topLeft.row(), 0).data(TabsModel::BackNavigatingRole).toBool());
    }

    if (roles.contains(TabsModel::Roles::ForwardNavigatingRole))
    {
        emit forwardNavigationAvailabilityChanged(_ctx->getTabsModel()->index(topLeft.row(), 0).data(TabsModel::ForwardNavigatingRole).toBool());
    }
}
