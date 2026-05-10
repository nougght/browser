#include "controllers/MainWindowController.h"
#include "controllers/TabsController.h"
#include "ui/mainwindow.h"

class AppBinder
{
public:
    AppBinder(MainWindow *mainWindow, MainWindowController *mainControler, TabsController *tabsController)
        : _mainWindow(mainWindow), _mainController(mainControler), _tabsController(tabsController)
    {
        QObject::connect(_tabsController, &TabsController::urlVisitRequested, _mainWindow, &MainWindow::visitUrl);
        QObject::connect(_tabsController, &TabsController::backNavigationRequested, _mainWindow, &MainWindow::navigateBack);
        QObject::connect(_tabsController, &TabsController::forwardNavigationRequested, _mainWindow, &MainWindow::navigateForward);
        QObject::connect(_tabsController, &TabsController::reloadRequested, _mainWindow, &MainWindow::reloadTab);

        QObject::connect(_tabsController, &TabsController::tabsLoaded, _mainWindow, &MainWindow::addTabs);
        QObject::connect(_tabsController, &TabsController::tabCreated, _mainWindow, &MainWindow::addTab);
        QObject::connect(_tabsController, &TabsController::activeTabChanged, _mainWindow, &MainWindow::switchToTab);

        QObject::connect(_tabsController, &TabsController::activeTabUrlChanged, _mainWindow, &MainWindow::updateUrlBar);
        QObject::connect(_tabsController, &TabsController::activeTabLoadingStatusChanged, _mainWindow, &MainWindow::setLoadingBarVisible);
        QObject::connect(_tabsController, &TabsController::activeTabLoadingProgressChanged, _mainWindow, &MainWindow::setLoadingProgress);
        QObject::connect(_tabsController, &TabsController::backNavigationAvailabilityChanged, _mainWindow, &MainWindow::setBackButtonEnabled);
        QObject::connect(_tabsController, &TabsController::forwardNavigationAvailabilityChanged, _mainWindow, &MainWindow::setForwardButtonEnabled);


        
        QObject::connect(_mainWindow, &MainWindow::newTabClicked, _tabsController, & TabsController::onNewTabClicked);
        QObject::connect(_mainWindow, &MainWindow::tabClicked, _tabsController, & TabsController::onTabClicked);
        QObject::connect(_mainWindow, &MainWindow::reloadClicked, _tabsController, & TabsController::onReloadClicked);
        QObject::connect(_mainWindow, &MainWindow::backClicked, _tabsController, & TabsController::onBackClicked);
        QObject::connect(_mainWindow, &MainWindow::forwardClicked, _tabsController, & TabsController::onForwardClicked);
        QObject::connect(_mainWindow, &MainWindow::searchClicked, _tabsController, & TabsController::onSearchRequested);
        QObject::connect(_mainWindow, &MainWindow::engineUrlChanged, _tabsController, & TabsController::onEngineUrlChanged);
        QObject::connect(_mainWindow, &MainWindow::engineTitleChanged, _tabsController, & TabsController::onEngineTitleChanged);
        QObject::connect(_mainWindow, &MainWindow::loadStarted, _tabsController, & TabsController::onLoadStarted);
        QObject::connect(_mainWindow, &MainWindow::loadFinished, _tabsController, & TabsController::onLoadFinished);
        QObject::connect(_mainWindow, &MainWindow::loadProgress, _tabsController, & TabsController::onLoadProgress);
    }

private:
    MainWindow *_mainWindow;
    MainWindowController *_mainController;
    TabsController *_tabsController;
};
