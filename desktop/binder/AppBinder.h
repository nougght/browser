#include "controllers/BookmarkController.h"
#include "controllers/HistoryController.h"
#include "controllers/MainWindowController.h"
#include "controllers/TabsController.h"
#include "ui/mainwindow.h"

// binds controller and ui signals
class AppBinder {
public:
    AppBinder(MainWindow *mainWindow, MainWindowController *mainController,
              TabsController *tabsController,
              BookmarkController *bookmarkController,
              HistoryController *historyController)
         {

        QObject::connect(tabsController, &TabsController::urlVisitCommand,
                         mainWindow, &MainWindow::visitUrl);
        QObject::connect(tabsController, &TabsController::backNavigationCommand,
                         mainWindow, &MainWindow::navigateBack);
        QObject::connect(tabsController,
                         &TabsController::forwardNavigationCommand, mainWindow,
                         &MainWindow::navigateForward);
        QObject::connect(tabsController, &TabsController::reloadCommand,
                         mainWindow, &MainWindow::reloadTab);
        
        QObject::connect(tabsController, &TabsController::navigationCompleted,
                         mainWindow, &MainWindow::onNavigationCompleted);

        QObject::connect(tabsController, &TabsController::tabsLoaded, mainWindow,
                         &MainWindow::addTabs);
        QObject::connect(tabsController, &TabsController::tabCreated, mainWindow,
                         &MainWindow::addTab);
        QObject::connect(tabsController, &TabsController::tabClosed, mainWindow,
                         &MainWindow::closeTab);
        QObject::connect(tabsController, &TabsController::lastTabClosed, mainWindow,
                         &MainWindow::close);

        QObject::connect(tabsController, &TabsController::activeTabChanged,
                         mainWindow, &MainWindow::switchToTab);

        QObject::connect(tabsController, &TabsController::activeTabUrlChanged,
                         mainWindow, &MainWindow::updateUrlBar);
        QObject::connect(tabsController,
                         &TabsController::activeTabLoadingStatusChanged,
                         mainWindow, &MainWindow::setLoadingBarVisible);
        QObject::connect(tabsController,
                         &TabsController::activeTabLoadingProgressChanged,
                         mainWindow, &MainWindow::setLoadingProgress);
        QObject::connect(tabsController,
                         &TabsController::backNavigationAvailabilityChanged,
                         mainWindow, &MainWindow::setBackButtonEnabled);
        QObject::connect(tabsController,
                         &TabsController::forwardNavigationAvailabilityChanged,
                         mainWindow, &MainWindow::setForwardButtonEnabled);




        QObject::connect(bookmarkController, &BookmarkController::switchActiveTabBookmark,
                         mainWindow, &MainWindow::switchActiveTabBookmark);



        QObject::connect(mainController,
                         &MainWindowController::historyPageRequested, mainWindow,
                         [mainWindow] { mainWindow->showInternalPage(InternalPageType::History); });
        QObject::connect(mainController,
                         &MainWindowController::bookmarksPageRequested, mainWindow,
                         [mainWindow] { mainWindow->showInternalPage(InternalPageType::Bookmarks); });



        QObject::connect(mainWindow, &MainWindow::newTabClicked, tabsController,
                         &TabsController::onNewTabClicked);
        QObject::connect(mainWindow, &MainWindow::tabClicked, tabsController,
                         &TabsController::onTabClicked);
        QObject::connect(mainWindow, &MainWindow::reloadClicked, tabsController,
                         &TabsController::onReloadClicked);
        QObject::connect(mainWindow, &MainWindow::backClicked, tabsController,
                         &TabsController::onBackClicked);
        QObject::connect(mainWindow, &MainWindow::forwardClicked, tabsController,
                         &TabsController::onForwardClicked);
        QObject::connect(mainWindow, &MainWindow::searchClicked, tabsController,
                         &TabsController::onSearchRequested);

        QObject::connect(mainWindow, &MainWindow::navigationRequested,
                         tabsController, &TabsController::onNavigationRequested);
        QObject::connect(mainWindow, &MainWindow::newTabRequested, tabsController,
                         &TabsController::onNewTabRequested);
        QObject::connect(mainWindow, &MainWindow::engineUrlChanged,
                         tabsController, &TabsController::onEngineUrlChanged);
        QObject::connect(mainWindow, &MainWindow::engineTitleChanged,
                         tabsController, &TabsController::onEngineTitleChanged);
        QObject::connect(mainWindow, &MainWindow::loadStarted, tabsController,
                         &TabsController::onLoadStarted);
        QObject::connect(mainWindow, &MainWindow::loadFinished, tabsController,
                         &TabsController::onLoadFinished);
        QObject::connect(mainWindow, &MainWindow::loadProgress, tabsController,
                         &TabsController::onLoadProgress);
        QObject::connect(mainWindow, &MainWindow::closeTabClicked, tabsController,
                         &TabsController::onCloseTabClicked);


        QObject::connect(mainWindow, &MainWindow::historyClicked, mainController,
                         &MainWindowController::onHistoryClicked);
        QObject::connect(mainWindow, &MainWindow::bookmarksClicked,
                         mainController,
                         &MainWindowController::onBookmarksClicked);


        QObject::connect(mainWindow, &MainWindow::bookmarkToggled, bookmarkController,
                         &BookmarkController::onActiveTabBookmakToggled);
        QObject::connect(mainWindow, &MainWindow::bookmarkEntryClicked, bookmarkController,
                         &BookmarkController::onBookmarkEntryClicked);
        QObject::connect(mainWindow, &MainWindow::deleteBookmarkEntryClicked, bookmarkController,
                         &BookmarkController::onDeleteBookmarkClicked);

        QObject::connect(mainWindow, &MainWindow::historyEntryClicked, historyController,
                         &HistoryController::onEntryClicked);
        QObject::connect(mainWindow, &MainWindow::deleteHistoryEntryClicked, historyController,
                         &HistoryController::onDeleteClicked);
        QObject::connect(mainWindow, &MainWindow::clearHistoryClicked, historyController,
                         &HistoryController::onClearClicked);
    }

};
