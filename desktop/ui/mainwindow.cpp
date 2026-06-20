#include "mainwindow.h"
#include "browsermenu.h"
#include "ui/webenginepage.h"
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QVBoxLayout>
#include <QWebEngineProfile>
#include <Qt>

#include <core/eventArgs.h>
#include <core/models.h>
#include <core/types.h>
#include <QWKWidgets/widgetwindowagent.h>

QUrl convert(Url url) {
    return QUrl(QString::fromStdString(url.toStdString()));
}

MainWindow::MainWindow(QAbstractListModel *tabsModel,
                       QAbstractListModel *historyModel,
                       QAbstractListModel *bookmarksModel) {
    setAttribute(Qt::WA_DeleteOnClose);
    setupUI(tabsModel, historyModel, bookmarksModel);
    setupEvents();
    qDebug() << QWebEngineProfile::defaultProfile()->httpUserAgent();
}

MainWindow::~MainWindow()
{
    // for (auto e : _tabWidgets) {
    //     e.second->deleteLater();
    // }
    // _tabWidgets.clear();
    // _profile->deleteLater();
    qDebug() << "main window destroyed";
}

void MainWindow::setupUI(QAbstractListModel *tabsModel,
                         QAbstractListModel *historyModel,
                         QAbstractListModel *bookmarksModel) {
    auto _rootLayout = new QVBoxLayout(this);
    _rootLayout->setSpacing(0);
    _rootLayout->setContentsMargins(0, 0, 0, 0);

    _centralWidget = new QFrame();
    _centralWidget->setObjectName("centralWidget");
    _centralLayout = new QVBoxLayout(_centralWidget);
    _centralLayout->setSpacing(0);
    _centralLayout->setContentsMargins(0, 0, 0, 0);

    _rootLayout->addWidget(_centralWidget);

    _profile = new QWebEngineProfile();
    connect(_profile, &QObject::destroyed, []{
        qDebug() << "profile destroyed";
    });

    this->setMouseTracking(true);
    _tabBar = new TabBarWithControl(_centralWidget, tabsModel);
    
    auto *agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    

    agent->setTitleBar(_tabBar);
    // кнопки панели управления
    agent->setSystemButton(QWK::WindowAgentBase::Minimize, _tabBar->minimiseButton());
    agent->setSystemButton(QWK::WindowAgentBase::Maximize, _tabBar->maximizeButton());
    agent->setSystemButton(QWK::WindowAgentBase::Close,     _tabBar->closeButton());
    // виджеты получающие mouse event (без перетаскивания окна)
    agent->setHitTestVisible(_tabBar->tabCountLabel(), true);
    agent->setHitTestVisible(_tabBar->tabsList(),      true);
    agent->setHitTestVisible(_tabBar->separator(), true);
    agent->setHitTestVisible(_tabBar->addNewTabButton(), true);
    agent->setHitTestVisible(_tabBar->minimiseButton(), true);
    agent->setHitTestVisible(_tabBar->maximizeButton(), true);
    agent->setHitTestVisible(_tabBar->closeButton(),    true);
    // agent->setWindowAttribute("mica", true);
    

    _menu = new BrowserMenu();
    _menu->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    _menu->setObjectName("browserMenu");
    _menu->style()->unpolish(_menu);
    _menu->style()->polish(_menu);

    _menu->ensurePolished();
    _historyPage = new HistoryPage(this, historyModel);
    _bookmarksPage = new BookmarksPage(this, bookmarksModel);

    // layout со строкой поиска

    _searchBarLayout = new QHBoxLayout();
    _searchBarLayout->setContentsMargins(0, 0, 8, 0);
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

    _search = new SearchBar();

    _menuButton = new QPushButton(QIcon(":/assets/grid.svg"), "");
    _menuButton->setObjectName("menuButton");
    _menuButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _menuButton->setEnabled(true);

    _searchBarLayout->addWidget(_backButton,
                                Qt::AlignmentFlag::AlignVCenter | Qt::AlignLeft);
    _searchBarLayout->addWidget(_forwardButton,
                                Qt::AlignmentFlag::AlignVCenter | Qt::AlignLeft);
    _searchBarLayout->addWidget(_reloadButton,
                                Qt::AlignmentFlag::AlignVCenter | Qt::AlignLeft);
    _searchBarLayout->addWidget(_search);
    _searchBarLayout->addWidget(_menuButton, Qt::AlignCenter);

    _prgsBarPlaceholder = new QWidget();
    _prgsBarPlaceholder->setFixedSize(10, 4);

    _loadingBar = new QProgressBar(_centralWidget);
    _loadingBar->hide();
    _loadingBar->setTextVisible(false);
    _loadingBar->setFormat("");

    _loadingBar->setMaximum(100);
    _loadingBar->setMinimum(0);

    _stackedWidget = new QStackedWidget(_centralWidget);
    _stackedWidget->addWidget(_historyPage);
    _stackedWidget->addWidget(_bookmarksPage);

    _centralLayout->addWidget(_tabBar);
    _centralLayout->addLayout(_searchBarLayout);
    _centralLayout->addWidget(_loadingBar, Qt::AlignmentFlag::AlignCenter);
    _centralLayout->addWidget(_prgsBarPlaceholder);
    _centralLayout->addWidget(_stackedWidget);
    this->setMinimumSize(500, 300);

    // TODO: for native gestures try https://github.com/stdware/qwindowkit
    // this->setWindowFlags(Qt::WindowFlags::enum_type::FramelessWindowHint);
    // this->setAttribute(Qt::WA_TranslucentBackground);

}

// setup ui signals
void MainWindow::setupEvents() {
    // отправляем сигналы при различных действиях пользователя
    connect(_search, &SearchBar::searchFinished, this,
            [this](QString query) { emit searchClicked(query); });

    connect(_search, &SearchBar::bookmarkToggled, this,
            [this]() { emit bookmarkToggled(); });

    connect(_tabBar, &TabBarWithControl::minimiseClicked, this,
            &MainWindow::showMinimized);
    connect(_tabBar, &TabBarWithControl::maximizedChanged, this,
        [this](bool isMaximized) { 
            if (isMaximized) {
                this->showMaximized();
            } else {
                this->showNormal();
            }
        });
    connect(_tabBar, &TabBarWithControl::closeClicked, this, &MainWindow::close);

    connect(_tabBar, &TabBarWithControl::newTabClicked, this,
            [this] { emit newTabClicked(); });
    connect(_tabBar, &TabBarWithControl::tabClicked, this,
            [this](int index) { emit tabClicked(index); });
    connect(_tabBar, &TabBarWithControl::tabCloseClicked, this,
            [this](int index) { emit closeTabClicked(index); });

    connect(_reloadButton, &QPushButton::clicked, this,
            [this] { emit reloadClicked(); });
    connect(_backButton, &QPushButton::clicked, this,
            [this] { emit backClicked(); });
    connect(_forwardButton, &QPushButton::clicked, this,
            [this] { emit forwardClicked(); });

    connect(_menuButton, &QPushButton::clicked, this, &MainWindow::menuClicked);

    connect(_menu, &BrowserMenu::newTabClicked, this,
            [this] { emit newTabClicked(); });
    connect(_menu, &BrowserMenu::historyClicked, this,
            [this] { emit historyClicked(); });
    connect(_menu, &BrowserMenu::bookmarksClicked, this,
            [this] { emit bookmarksClicked(); });

    // connect(_tabBar, &TabBarWithControl::barDoubleClicked, this,
    // &MainWindow::switch)


    // history page
    connect(_historyPage, &HistoryPage::historyClicked, this,
            [this](int index) { emit historyEntryClicked(index); });
    connect(_historyPage, &HistoryPage::deleteClicked, this,
            [this](int index) { emit deleteHistoryEntryClicked(index); });
    connect(_historyPage, &HistoryPage::clearClicked, this,
            [this] { emit clearHistoryClicked(); });

    // bookmarks page
    connect(_bookmarksPage, &BookmarksPage::bookmarkClicked, this,
            [this](int index) { emit bookmarkEntryClicked(index); });
    connect(_bookmarksPage, &BookmarksPage::deleteClicked, this,
            [this](int index) { emit deleteBookmarkEntryClicked(index); });
}

// setup signals for webengine
void MainWindow::setupTabViewEvents(TabId tabId, QWebEngineView *tabView) {
    tabView->setObjectName("tabView");
    // настраиваем сигналы при различных событиях от движка
    connect(tabView->page(), &QWebEnginePage::urlChanged, this,
            [this, id = tabId](QUrl url) { emit engineUrlChanged(id, url); });

    connect(tabView->page(), &QWebEnginePage::titleChanged, this,
            [this, id = tabId](QString title) {
        emit engineTitleChanged(id, title);
    });

    connect(tabView->page(), &QWebEnginePage::loadStarted, this,
            [this, id = tabId]() { emit loadStarted(id); });

    connect(tabView->page(), &QWebEnginePage::loadFinished, this,
            [this, id = tabId]() { emit loadFinished(id); });

    connect(
        tabView->page(), &QWebEnginePage::loadProgress, this,
        [this, id = tabId](int progress) { emit loadProgress(id, progress); });


    connect(tabView, &QObject::destroyed, []{
        qDebug() << "view destroyed";
    });
    connect(tabView->page(), &QObject::destroyed, [] {
        qDebug() << "page destroyed";
    });
}

void MainWindow::visitUrl(TabInfo tab, bool isBookmarked) {
    _tabViews[tab.id]->load(convert(tab.url));
    switchActiveTabBookmark(isBookmarked);
}

void MainWindow::showHistoryPage() {
    qDebug() << "show history";
    _stackedWidget->setCurrentWidget(_historyPage);
}

void MainWindow::showBookmarksPage() {
    qDebug() << "show bookmarks";
    _stackedWidget->setCurrentWidget(_bookmarksPage);
}

void MainWindow::reloadTab(TabId id) { _tabViews[id]->reload(); }

void MainWindow::navigateBack(TabInfo tabInfo, bool isBookmarked) {
    _tabViews[tabInfo.id]->back();
    switchActiveTabBookmark(isBookmarked);
    // _tabBar->updateTabNavigation(tabInfo.id, tabInfo.canGoBack,
    // tabInfo.canGoForward);
}
void MainWindow::navigateForward(TabInfo tabInfo, bool isBookmarked) {
    _tabViews[tabInfo.id]->forward();
    switchActiveTabBookmark(isBookmarked);
    // _tabBar->updateTabNavigation(tabInfo.id, tabInfo.canGoBack,
    // tabInfo.canGoForward);
}

// add multiple tabs
void MainWindow::addTabs(std::vector<TabInfo> tabs) {
    for (int i = 0; i < tabs.size(); ++i) {
        auto view = new QWebEngineView();
        auto page = new QWebEnginePage(_profile, view);
        view->setPage(page);

        _stackedWidget->addWidget(view);

        setupTabViewEvents(tabs[i].id, view);

        view->load(convert(tabs[i].url));
        _tabViews[tabs[i].id] = std::move(view);
    }
    TabId lastId = tabs.back().id;
    // _tabBar->setInitialTabs(std::move(tabs));
}

// add single tab
void MainWindow::addTab(TabInfo tabInfo) {
    // _tabBar->addTab(tabInfo);

    auto view = new QWebEngineView(this);
    auto page = new WebEnginePage(_profile, view);
    view->setPage(page);

    _stackedWidget->addWidget(view);

    setupTabViewEvents(tabInfo.id, view);
    view->load(convert(tabInfo.url));
    _tabViews[tabInfo.id] = std::move(view);

}

void MainWindow::closeTab(TabId id) {
    if (_tabViews.count(id) == 0)
        return;

    auto tab = _tabViews[id];
    _stackedWidget->removeWidget(tab);
    _tabViews.erase(id);
    tab->deleteLater();
}


// switch active tab
void MainWindow::switchToTab(TabInfo tabInfo, bool isBookmarked) {
    auto it = _tabViews.find(tabInfo.id);
    if (it != _tabViews.end()) {
        _stackedWidget->setCurrentWidget(it->second);

        it->second->setFocus();
        // _activeTabId = tabId;

        _backButton->setEnabled(tabInfo.canGoBack);
        _forwardButton->setEnabled(tabInfo.canGoForward);

        switchActiveTabBookmark(isBookmarked);
        // QTimer::singleShot(1, this, [=]() {
        //     QMetaObject::invokeMethod(
        //         this,
        //         [=]() {
        //             _tabBar->setTabSelected(std::distance(_tabWidgets.begin(), it));

        //             qDebug() << "\nui switched active tab\n";
        //         },
        //         Qt::QueuedConnection);
        // });
    }
}

void MainWindow::setLoadingBarVisible(bool isVisible) {
    _prgsBarPlaceholder->setVisible(!isVisible);
    _loadingBar->setVisible(isVisible);
}

void MainWindow::setLoadingProgress(int progress) {
    _loadingBar->setValue(progress);
}

void MainWindow::setBackButtonEnabled(bool enabled) {
    _backButton->setEnabled(enabled);
}

void MainWindow::setForwardButtonEnabled(bool enabled) {
    _forwardButton->setEnabled(enabled);
}

void MainWindow::updateUrlBar(QUrl newUrl) {
    qDebug() << "update url bar: " << newUrl.toString();
    if (_search->text() != newUrl) {
        QSignalBlocker blocker(_search);
        _search->setText(newUrl.toString());
        _search->setCursorPosition(0);
    }
}

void MainWindow::menuClicked() {
    qDebug() << "menu clicked\n";
    auto p = QWidget::mapToGlobal(_menuButton->geometry().bottomLeft());
    qDebug() << "\n" << p.x() << " " << p.y();
    _menu->exec(QPoint(p.x() - 200, p.y()));
}

void MainWindow::switchActiveTabBookmark(bool isBookmarked) {
    qDebug() << "\nMW switch tab bookmark  " << isBookmarked << "\n";
    _search->switchBookmark(isBookmarked);
}

