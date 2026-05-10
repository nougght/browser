#include "CoreAdapter.h"
#include <memory>


void CoreAdapter::_setupEvents()
{
    _subscriptions.push_back(std::make_unique<Subscription<std::vector<TabInfo>>>(
        _core->tabsLoaded.subscribe(
            [this](std::vector<TabInfo> tabs)
            {
                QMetaObject::invokeMethod(this, [this, tabs = std::move(tabs)]()
                                          { emit this->tabsLoaded(tabs); });
            })));

    _subscriptions.push_back(std::make_unique<Subscription<TabInfo>>(
        _core->tabCreated.subscribe(
            [this](TabInfo tabInfo)
            {
                QMetaObject::invokeMethod(this, [this, tabInfo]()
                                          { emit this->tabCreated(tabInfo); }, Qt::QueuedConnection);
            })));

    _subscriptions.push_back(std::make_unique<Subscription<TabId>>(
        _core->activeTabChanged.subscribe(
            [this](TabId id)
            {
                QMetaObject::invokeMethod(this, [this, id]()
                                          { emit this->activeTabChanged(id); }, Qt::QueuedConnection);
            })));

    _subscriptions.push_back(std::make_unique<Subscription<NavigationRequestedArgs>>(
        _core->navigationRequested.subscribe(
            [this](NavigationRequestedArgs args)
            {
                QMetaObject::invokeMethod(this, [this, args]()
                                          { emit this->navigationRequested(args); }, Qt::QueuedConnection);
            })));

    _subscriptions.push_back(std::make_unique<Subscription<TabTitleChangedArgs>>(
        _core->titleChanged.subscribe(
            [this](TabTitleChangedArgs args)
            {
                QMetaObject::invokeMethod(this, [this, args]()
                                          { emit this->titleChanged(args); }, Qt::QueuedConnection);
            })));

    _subscriptions.push_back(std::make_unique<Subscription<TabLoadingStatusChangedArgs>>(
        _core->loadingStatusChanged.subscribe(
            [this](TabLoadingStatusChangedArgs args)
            {
                QMetaObject::invokeMethod(this, [this, args]()
                                          { emit this->loadingStatusChanged(args); }, Qt::QueuedConnection);
            })));

    _subscriptions.push_back(std::make_unique<Subscription<TabLoadingProgressChangedArgs>>(
        _core->loadingProgessChanged.subscribe(
            [this](TabLoadingProgressChangedArgs args)
            {
                QMetaObject::invokeMethod(this, [this, args]()
                                          { emit this->loadingProgressChanged(args); }, Qt::QueuedConnection);
            })));

}


void CoreAdapter::loadTabs()
{
    _core->loadTabs();
}

void CoreAdapter::createTab(QUrl url)
{
    _core->createTab(convert(url));
}


void CoreAdapter::createTab()
{
    _core->createTab();
}

void CoreAdapter::closeTab(TabId id)
{
    _core->closeTab(id);
}

void CoreAdapter::changeActiveTab(TabId id)
{
    _core->changeActiveTab(id);
}

void CoreAdapter::moveTab(TabId id, int newIndex)
{
    _core->moveTab(id, newIndex);
}

void CoreAdapter::goForward(TabId id)
{
    _core->goForward(id);
}

void CoreAdapter::goBack(TabId id)
{
    _core->goBack(id);
}


void CoreAdapter::visitUrl(TabId id, QUrl url)
{
    _core->visitUrl(id, convert(url));
}

void CoreAdapter::changeTabUrl(TabId id, QUrl url)
{
    _core->changeTabUrl(id, convert(url));
}

void CoreAdapter::changeTabTitle(TabId id, QString title)
{
    _core->changeTabTitle(id, title.toStdString());
}

void CoreAdapter::changeTabLoadingProgress(TabId id, int progress)
{
    _core->changeTabLoadingProgress(id, progress);
}

void CoreAdapter::setTabLoadingStatus(TabId id, bool isLoading)
{
    _core->setTabLoadingStatus(id, isLoading);
}

void CoreAdapter::reloadTab(TabId id)
{
    _core->reloadTab(id);
}

