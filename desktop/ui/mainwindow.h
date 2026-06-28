#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QWebEngineView>

#include <QProgressBar>
#include <QStackedWidget>
#include <QVBoxLayout>

#include <map>

#include "bookmarkspage.h"
#include "browsermenu.h"
#include "core/Identifier.h"
#include "historypage.h"
#include "searchbar.h"
#include "tabbarwithcontrol.h"

// done: выделить контроллер из виджета и оставить только ui

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QAbstractListModel *tabsModel, QAbstractListModel *historyModel,
               QAbstractListModel *bookmarksModel);
    ~MainWindow();

signals:
    void minimiseClicked();
    void closeClicked();

    void newTabClicked();
    void tabClicked(int index);
    void closeTabClicked(int index);
    void reloadClicked();
    void backClicked();
    void forwardClicked();

    void historyClicked();
    void historyEntryClicked(int index);
    void deleteHistoryEntryClicked(int index);
    void clearHistoryClicked();

    
    void searchClicked(QString searchQuery);
    void engineUrlChanged(TabId id, QUrl newUrl);
    void engineTitleChanged(TabId id, QString newTitle);
    void loadStarted(TabId id);
    void loadFinished(TabId id);
    void loadProgress(TabId id, int progress);

    void bookmarkToggled();
    void bookmarksClicked();
    void bookmarkEntryClicked(int index);
    void deleteBookmarkEntryClicked(int index);

public slots:

    void addTabs(std::vector<TabInfo> tabs);
    void addTab(TabInfo tabInfo);
    void closeTab(TabId id);
    void switchToTab(TabInfo tabInfo, bool isBookmarked);

    void setLoadingBarVisible(bool isVisible);
    void setLoadingProgress(int progress);
    void setBackButtonEnabled(bool enabled);
    void setForwardButtonEnabled(bool enabled);

    void updateUrlBar(Url newUrl);
    // void updateTabTitle(TabId id, std::string title);
    void reloadTab(TabId id);
    void navigateBack(TabInfo tabInfo, bool isBookmarked);
    void navigateForward(TabInfo tabInfo, bool isBookmarked);
    void visitUrl(TabInfo tab, bool isBookmarked);


    // TODO: move to navigation controller
    void showHistoryPage();
    void showBookmarksPage();

    void switchActiveTabBookmark(bool isBookmarked);

private:
    void setupUI(QAbstractListModel *tabsModel, QAbstractListModel *historyModel,
                 QAbstractListModel *bookmarksModel);
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
    BookmarksPage *_bookmarksPage;

    QWebEngineProfile* _profile;
    QStackedWidget *_stackedWidget;

    std::map<TabId, QWebEngineView *> _tabViews;
};
#endif // MAINWINDOW_H
