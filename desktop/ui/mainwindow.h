#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QWebEngineView>

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QProgressBar>

#include <map>

#include "tabbarwithcontrol.h"
#include "browsermenu.h"
#include "historypage.h"
#include "searchbar.h"
#include "core/Identifier.h"

// to do: выделить контроллер из виджета и оставить только ui

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QAbstractListModel *tabsModel, QAbstractListModel *historyModel);
    ~MainWindow();

signals:
    void minimiseClicked();
    void closeClicked();


    void newTabClicked();
    void tabClicked(int index);
    void reloadClicked();
    void backClicked();
    void forwardClicked();

    void historyClicked();

    void searchClicked(QString searchQuery);
    void engineUrlChanged(TabId id, QUrl newUrl);
    void engineTitleChanged(TabId id, QString newTitle);
    void loadStarted(TabId id);
    void loadFinished(TabId id);
    void loadProgress(TabId id, int progress);

    void bookmarkToggled();

public slots:

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

    void showHistoryPage();


private:
    void setupUI(QAbstractListModel *tabsModel, QAbstractListModel *historyModel);
    void setupEvents();
    void setupTabViewEvents(TabId tabId, QWebEngineView *tabView);

    void menuClicked();


    QWidget *_centralWidget;
    QVBoxLayout *_centralLayout;
    QHBoxLayout *_searchBarLayout;
    SearchBar *_search;
    QPushButton *_reloadButton;
    QPushButton *_backButton;
    QPushButton *_forwardButton;
    QPushButton *_menuButton;
    TabBarWithControl *_tabBar;
    QProgressBar *_loadingBar;
    QWidget *_prgsBarPlaceholder;

    BrowserMenu *_menu;
    HistoryPage *_historyPage;
    QStackedWidget *_stackedWidget;

    std::map<TabId, QWebEngineView *> _tabWidgets;
};
#endif // MAINWINDOW_H
