#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QProgressBar>

#include <memory>
#include <map>

#include "tabbarwithcontrol.h"
#include "../models/tabsmodel.h"

#include "core/BrowserCore.h"
#include "core/Url.h"
#include "core/Identifier.h"
#include "core/Event.h"

// to do: выделить контроллер из виджета и оставить только ui

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QAbstractListModel *tabsModel);
    ~MainWindow();

signals:
    void minimiseClicked();
    void closeClicked();


    void newTabClicked();
    void tabClicked(int index);
    void reloadClicked();
    void backClicked();
    void forwardClicked();


    void searchClicked(QString searchQuery);
    void engineUrlChanged(TabId id, QUrl newUrl);
    void engineTitleChanged(TabId id, QString newTitle);
    void loadStarted(TabId id);
    void loadFinished(TabId id);
    void loadProgress(TabId id, int progress);

public slots:
    // void onTabsLoaded(std::vector<TabInfo>);
    // void onTabCreated(TabInfo tabInfo);
    // void onNavigationCompleted(NavigationCompletedArgs args);
    // void onSearchEditingFinished();
    // void onEngineUrlChanged(TabId id, QUrl newUrl);
    // void onEngineTitleChanged(TabId id, QString newTitle);
    // void onLoadingStatusChanged(TabId id, bool isLoading);
    // void onLoadingProgressChanged(TabId id, int progress);

    void addTabs(std::vector<TabInfo> tabs);
    void addTab(TabInfo tabInfo);
    void switchToTab(TabInfo tabInfo);

    void setLoadingBarVisible(bool isVisible);
    void setLoadingProgress(int progress);
    void setBackButtonEnabled(bool enabled);
    void setForwardButtonEnabled(bool enabled);

    void updateUrlBar(QUrl newUrl);
    // void updateTabTitle(TabId id, std::string title);
    void reloadTab(TabId id);
    void navigateBack(TabInfo tabInfo);
    void navigateForward(TabInfo tabInfo);
    void visitUrl(TabInfo tab);

    // void onTabsModelDataChanged(const QModelIndex &topLeft,
    //                             const QModelIndex &bottomRight,
    //                             const QList<int> &roles = QList<int>());

private:
    void setupUI(QAbstractListModel *tabsModel);
    void setupEvents();
    void setupTabViewEvents(TabId tabId, QWebEngineView *tabView);

    // TabsModel *_tabsModel;

    QWidget *_centralWidget;
    QVBoxLayout *_centralLayout;
    QHBoxLayout *_searchBarLayout;
    QLineEdit *_search;
    QPushButton *_reloadButton;
    QPushButton *_backButton;
    QPushButton *_forwardButton;
    TabBarWithControl *_tabBar;
    QProgressBar *_loadingBar;
    QWidget *_prgsBarPlaceholder;
    QStackedWidget *_stackedWidget;

    // std::unique_ptr<BrowserCore> _core;
    // TabId _activeTabId;
    std::map<TabId, QWebEngineView *> _tabWidgets;
};
#endif // MAINWINDOW_H
