#include "mainwindow.h"
#include <QVBoxLayout>
#include <core/BrowserCore.h>
#include <core/models.h>
#include <core/types.h>
#include <core/eventArgs.h>
#include <QWebEngineProfile>
#include <Qt>

QUrl convert(Url url)
{
    return QUrl(QString::fromStdString(url.toStdString()));
}

MainWindow::MainWindow(QAbstractListModel *tabsModel)
{
    // _tabsModel = new TabsModel(this);
    setupUI(tabsModel);
    setupEvents();
    qDebug() << QWebEngineProfile::defaultProfile()->httpUserAgent();
}

void MainWindow::setupUI(QAbstractListModel *tabsModel)
{
    _centralWidget = new QFrame();
    _centralWidget->setObjectName("centralWidget");
    _centralLayout = new QVBoxLayout(_centralWidget);
    _centralLayout->setSpacing(0);
    // _centralLayout->
    _centralLayout->setContentsMargins(0, 0, 0, 0);
    this->setCentralWidget(_centralWidget);

    
    _tabBar = new TabBarWithControl(_centralWidget, tabsModel);

    _searchBarLayout = new QHBoxLayout();
    _searchBarLayout->setContentsMargins(0, 0, 0, 0);
    _searchBarLayout->setSpacing(0);

    _backButton = new QPushButton("🠔", _centralWidget);
    _backButton->setObjectName("backButton");
    _backButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _forwardButton = new QPushButton("🠖", _centralWidget);
    _forwardButton->setObjectName("forwardButton");
    _forwardButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _reloadButton = new QPushButton("⟳", _centralWidget);
    _reloadButton->setObjectName("reloadButton");
    _reloadButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _search = new QLineEdit(_centralWidget);
    _search->setObjectName("searchBar");
    _search->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    _searchBarLayout->addWidget(_backButton, Qt::AlignmentFlag::AlignVCenter | Qt::AlignLeft);
    _searchBarLayout->addWidget(_forwardButton, Qt::AlignmentFlag::AlignVCenter | Qt::AlignLeft);
    _searchBarLayout->addWidget(_reloadButton, Qt::AlignmentFlag::AlignVCenter | Qt::AlignLeft);
    _searchBarLayout->addWidget(_search);

    _prgsBarPlaceholder = new QWidget();
    _prgsBarPlaceholder->setFixedSize(10, 4);

    _loadingBar = new QProgressBar(_centralWidget);
    _loadingBar->hide();
    _loadingBar->setTextVisible(false);
    _loadingBar->setFormat("");

    _loadingBar->setMaximum(100);
    _loadingBar->setMinimum(0);

    _stackedWidget = new QStackedWidget(_centralWidget);
    // _page = new QWebEngineView(_centralWidget);

    _centralLayout->addWidget(_tabBar);
    _centralLayout->addLayout(_searchBarLayout);
    _centralLayout->addWidget(_loadingBar, Qt::AlignmentFlag::AlignCenter);
    _centralLayout->addWidget(_prgsBarPlaceholder);
    _centralLayout->addWidget(_stackedWidget);
    this->setMinimumSize(1000, 700);
    this->setWindowFlags(Qt::WindowFlags::enum_type::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void MainWindow::setupEvents()
{
    // подписка на события из core
    // добавляем обработчик через invokeMethod, чтобы передать его в Qt поток

    // _subscriptions.push_back(std::make_unique<Subscription<std::vector<TabInfo>>>(
    //     _core->tabsLoaded.subscribe([this](std::vector<TabInfo> tabs)
    //                                 { QMetaObject::invokeMethod(this, [this, tabs = std::move(tabs)]()
    //                                                             { this->onTabsLoaded(std::move(tabs)); }); })));

    // _subscriptions.push_back(std::make_unique<Subscription<TabInfo>>(_core->tabCreated.subscribe(
    //     [this](TabInfo tabInfo)
    //     { QMetaObject::invokeMethod(this, [this, tabInfo]()
    //                                 { this->onTabCreated(tabInfo); }, Qt::QueuedConnection); })));

    // _subscriptions.push_back(std::make_unique<Subscription<TabId>>(_core->activeTabChanged.subscribe(
    //     [this](TabId id)
    //     { QMetaObject::invokeMethod(this, [this, id]()
    //                                 { this->onActiveTabChanged(id); }, Qt::QueuedConnection); })));

    // _subscriptions.push_back(std::make_unique<Subscription<NavigationCompletedArgs>>(
    //     _core->navigationCompleted.subscribe(
    //         [this](NavigationCompletedArgs args)
    //         { QMetaObject::invokeMethod(this, [this, args]()
    //                                     { this->onNavigationCompleted(args); }, Qt::QueuedConnection); })));

    // _subscriptions.push_back(std::make_unique<Subscription<TabTitleChangedArgs>>(
    //     _core->titleChanged.subscribe(
    //         [this](TabTitleChangedArgs args)
    //         { QMetaObject::invokeMethod(this, [this, args]()
    //                                     { this->updateTabTitle(args.id, args.newTitle); }, Qt::QueuedConnection); })));

    // _subscriptions.push_back(std::make_unique<Subscription<TabLoadingStatusChangedArgs>>(
    //     _core->loadingStatusChanged.subscribe(
    //         [this](TabLoadingStatusChangedArgs args)
    //         { QMetaObject::invokeMethod(this, [this, args]()
    //                                     { this->onLoadingStatusChanged(args.id, args.isLoading); }, Qt::QueuedConnection); })));

    // _subscriptions.push_back(std::make_unique<Subscription<TabLoadingProgressChangedArgs>>(
    //     _core->loadingProgessChanged.subscribe(
    //         [this](TabLoadingProgressChangedArgs args)
    //         { QMetaObject::invokeMethod(this, [this, args]()
    //                                     { this->onLoadingProgressChanged(args.id, args.progress); }, Qt::QueuedConnection); })));

    connect(_search, &QLineEdit::returnPressed, this,
            [this]
            { emit searchClicked(_search->text()); });

    // подписка на событие из core
    // auto sub = ;

    // // auto sub2 = ;
    // _subscriptions.push_back(std::make_unique<Subscription<UrlChangedArgs>>(_core->urlChanged.subscribe(
    //     [this](UrlChangedArgs args)
    //     { this->updateUrlBar(QString::fromStdString(args.url.toStdString())); })));

    connect(_tabBar, &TabBarWithControl::minimiseClicked, this, &MainWindow::showMinimized);
    connect(_tabBar, &TabBarWithControl::closeClicked, this, &MainWindow::close);

    connect(_tabBar, &TabBarWithControl::newTabClicked, this, [this]
            { emit newTabClicked(); });
    connect(_tabBar, &TabBarWithControl::tabClicked, this, [this](int index)
            { emit tabClicked(index); });

    connect(_reloadButton, &QPushButton::clicked, this, [this]
            { emit reloadClicked(); });
    connect(_backButton, &QPushButton::clicked, this, [this]
            { emit backClicked(); });
    connect(_forwardButton, &QPushButton::clicked, this, [this]
            { emit forwardClicked(); });

    // connect(_tabsModel, &QAbstractItemModel::dataChanged, this, &MainWindow::onTabsModelDataChanged);
}

// void MainWindow::onNewTabClicked()
// {
//     _core->createTab();
// }

// void MainWindow::onTabClicked(TabId id)
// {
//     _core->changeActiveTab(id);
// }

// void MainWindow::onReloadClicked()
// {
//     _core->reloadTab(_activeTabId);
// }

// void MainWindow::onBackClicked()
// {
//     _core->goBack(_activeTabId);
// }

// void MainWindow::onForwardClicked()
// {
//     _core->goForward(_activeTabId);
// }

// void MainWindow::onLoadingStatusChanged(TabId id, bool isLoading)
// {
//     if (id == _activeTabId)
//     {
//         setLoadingBarVisible(isLoading);
//     }
// }

void MainWindow::setLoadingBarVisible(bool isVisible)
{
    _prgsBarPlaceholder->setVisible(!isVisible);
    _loadingBar->setVisible(isVisible);
}

void MainWindow::setLoadingProgress(int progress)
{
    _loadingBar->setValue(progress);
}

void MainWindow::setBackButtonEnabled(bool enabled)
{
    _backButton->setEnabled(enabled);
}

void MainWindow::setForwardButtonEnabled(bool enabled)
{
    _forwardButton->setEnabled(enabled);
}

void MainWindow::updateUrlBar(QUrl newUrl)
{
    if (_search->text() != newUrl)
    {
        QSignalBlocker blocker(_search);
        _search->setText(newUrl.toString());
        _search->setCursorPosition(0);
    }
}

// void MainWindow::onNavigationCompleted(NavigationCompletedArgs args)
// {
//     qDebug() << "\nNavigationHandlers " << _core->navigationCompleted.handlersSize() << "\n";
//     switch (args.type)
//     {
//     case NavigationType::NewPage:
//         onUrlVisited(args.tabInfo);
//         break;
//     case NavigationType::Back:
//         navigateBack(args.tabInfo);
//         break;
//     case NavigationType::Forward:
//         navigateForward(args.tabInfo);
//         break;
//     case NavigationType::Redirect:
//         updateUrlBar(convert(args.tabInfo.url));
//         // _tabBar->updateTabNavigation(_activeTabId, args.tabInfo.canGoBack, args.tabInfo.canGoForward);
//         break;
//     case NavigationType::Reload:
//         // onUrlVisited(convert(args.tabInfo.url));
//         reloadTab(args.tabInfo.id);
//         break;
//     }
// }

// void MainWindow::updateTabTitle(TabId id, std::string title)
// {
//     _tabBar->updateTabTitle(id, title);
// }


void MainWindow::visitUrl(TabInfo tab)
{
    _tabWidgets[tab.id]->load(convert(tab.url));
}

void MainWindow::reloadTab(TabId id)
{
    _tabWidgets[id]->reload();
}

void MainWindow::navigateBack(TabInfo tabInfo)
{
    _tabWidgets[tabInfo.id]->back();
    // _tabBar->updateTabNavigation(tabInfo.id, tabInfo.canGoBack, tabInfo.canGoForward);
}
void MainWindow::navigateForward(TabInfo tabInfo)
{
    _tabWidgets[tabInfo.id]->forward();
    // _tabBar->updateTabNavigation(tabInfo.id, tabInfo.canGoBack, tabInfo.canGoForward);
}

void MainWindow::setupTabViewEvents(TabId tabId, QWebEngineView *tabView)
{
    connect(tabView, &QWebEngineView::urlChanged, this,
            [this, id = tabId](QUrl url)
            {
                emit engineUrlChanged(id, url);
            });

    connect(tabView, &QWebEngineView::titleChanged, this,
            [this, id = tabId](QString title)
            {
                emit engineTitleChanged(id, title);
            });

    connect(tabView, &QWebEngineView::loadStarted, this,
            [this, id = tabId]()
            {
                emit loadStarted(id);
            });

    connect(tabView, &QWebEngineView::loadFinished, this,
            [this, id = tabId]()
            {
                emit loadFinished(id);
            });

    connect(tabView, &QWebEngineView::loadProgress, this,
            [this, id = tabId](int progress)
            {
                emit loadProgress(id, progress);
            });
}

// add multiple tabs
void MainWindow::addTabs(std::vector<TabInfo> tabs)
{
    for (int i = 0; i < tabs.size(); ++i)
    {
        auto tabWidget = new QWebEngineView();

        _stackedWidget->addWidget(tabWidget);

        setupTabViewEvents(tabs[i].id, tabWidget);

        tabWidget->load(convert(tabs[i].url));
        _tabWidgets[tabs[i].id] = std::move(tabWidget);
    }
    TabId lastId = tabs.back().id;
    // _tabBar->setInitialTabs(std::move(tabs));

}

// add single tab
void MainWindow::addTab(TabInfo tabInfo)
{
    // _tabBar->addTab(tabInfo);

    auto tabWidget = new QWebEngineView();

    _stackedWidget->addWidget(tabWidget);

    setupTabViewEvents(tabInfo.id, tabWidget);
    tabWidget->load(convert(tabInfo.url));
    _tabWidgets[tabInfo.id] = std::move(tabWidget);

    switchToTab(tabInfo);
}

// switch active tab
void MainWindow::switchToTab(TabInfo tabInfo)
{
    auto it = _tabWidgets.find(tabInfo.id);
    if (it != _tabWidgets.end())
    {
        // находим индекс виджета и переключаемся
        int index = _stackedWidget->indexOf(it->second);
        _stackedWidget->setCurrentIndex(index);

        it->second->setFocus();
        // _activeTabId = tabId;

        _backButton->setEnabled(tabInfo.canGoBack);
        _forwardButton->setEnabled(tabInfo.canGoForward);
    }
}

// void MainWindow::onEngineUrlChanged(TabId id, QUrl newUrl)
// {
//     // QMetaObject::invokeMethod(this, [this, newUrl]{
//     _core->changeTabUrl(id, Url(newUrl.toString().toStdString()));
//     // }, Qt::QueuedConnection);
// }

// void MainWindow::onEngineTitleChanged(TabId id, QString newTitle)
// {
//     _core->changeTabTitle(id, newTitle.toStdString());
// }

// void MainWindow::onTabsModelDataChanged(const QModelIndex &topLeft,
//                                         const QModelIndex &bottomRight,
//                                         const QList<int> &roles)
// {
//     if (roles.contains(TabsModel::Roles::BackNavigatingRole))
//     {
//         _backButton->setEnabled(_tabsModel->index(topLeft.row(), 0).data(TabsModel::BackNavigatingRole).toBool());
//     }

//     if (roles.contains(TabsModel::Roles::ForwardNavigatingRole))
//     {
//         _forwardButton->setEnabled(_tabsModel->index(topLeft.row(), 0).data(TabsModel::ForwardNavigatingRole).toBool());
//     }
// }

// void MainWindow::onSearchEditingFinished()
// {
//     // goToWebsite(_search->text());

//     // qDebug() << "\nSIZE = " << _core->urlVisited.handlersSize();
//     _core->visitUrl(_activeTabId, Url(_search->text().toStdString()));
// }

// void MainWindow::onUrlVisited(TabInfo tab)
// {
//     _tabBar->updateTabUrl(_activeTabId, tab.url);
//     _tabBar->updateTabNavigation(_activeTabId, tab.canGoBack, tab.canGoForward);
//     _tabWidgets[_activeTabId]->load(convert(tab.url));
// }


MainWindow::~MainWindow() {}
