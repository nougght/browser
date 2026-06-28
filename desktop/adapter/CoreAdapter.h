#ifndef CORE_ADAPTER_H
#define CORE_ADAPTER_H

#include <memory>


#include "core/interfaces/IBrowserCore.h"
#include "core/Url.h"
#include "core/Identifier.h"
#include "core/Event.h"
#include <QObject>
#include <QUrl>

// QT wrapper for core (converts core events to Qt signals)
class CoreAdapter: public QObject
{
    Q_OBJECT

public:
    CoreAdapter(IBrowserCore *core) : _core(core)
    {
        _setupEvents();
    }
    QUrl static convert(Url url)
    {
        return QUrl(QString::fromStdString(url.toStdString()));
    }
    Url static convert(QUrl url)
    {
        auto optionalUrl = Url::parse(url.toString().toStdString());
        if (!optionalUrl.has_value()) {
            throw std::runtime_error("CoreAdapter convert QUrl to Url failed");
        }
        return optionalUrl.value();
    }

signals:
    // tabs
    void tabsLoaded(std::vector<TabInfo> tabs);
    void tabCreated(TabInfo tab);
    void tabClosed(TabId id);
    void lastTabClosed();
    void activeTabChanged(TabId id);
    void navigationRequested(NavigationRequestedArgs args);
    void titleChanged(TabTitleChangedArgs args);
    void loadingStatusChanged(TabLoadingStatusChangedArgs args);
    void loadingProgressChanged(TabLoadingProgressChangedArgs args);

    // history
    void historyLoaded(std::vector<HistoryEntry> history);
    void historyEntryAdded(HistoryEntry entry);
    void historyEntryDeleted(int64_t id);
    void historyCleared();

    // bookmarks
    void bookmarksLoaded(std::vector<Bookmark> bookmarks);
    void bookmarkAdded(Bookmark bookmark);
    void bookmarkDeleted(int64_t id);

public slots:
    void loadTabs();
    void createTab(QUrl url);
    void createTab();
    void closeTab(TabId id);
    void changeActiveTab(TabId id);
    void moveTab(TabId id, int newIndex);
    void goForward(TabId id);
    void goBack(TabId id);
    void handleSearchQuery(TabId id, std::string query);
    void openInternalPage(InternalPageType type, bool isNewTab);
    void changeTabUrl(TabId id, QUrl url);
    void changeTabTitle(TabId id, QString title);
    void changeTabLoadingProgress(TabId id, int progress);
    void setTabLoadingStatus(TabId id, bool isLoading);

    void reloadTab(TabId id);

    void loadHistory();
    void deleteHistoryEntry(int64_t id);
    void clearHistory();

    void loadBookmarks();
    void switchActiveTabBookmark();
    void addBookmark(Bookmark bookmark);
    void deleteBookmark(int64_t id);

private:
    void _setupEvents();
    std::vector<std::unique_ptr<ISubscription>> _subscriptions;
    IBrowserCore *_core;
};
#endif
