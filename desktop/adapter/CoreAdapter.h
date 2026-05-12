#ifndef CORE_ADAPTER_H
#define CORE_ADAPTER_H

#include <memory>


#include "core/BrowserCore.h"
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
    CoreAdapter(BrowserCore *core) : _core(core)
    {
        _setupEvents();
    }
    QUrl static convert(Url url)
    {
        return QUrl(QString::fromStdString(url.toStdString()));
    }
    Url static convert(QUrl url)
    {
        return Url(url.toString().toStdString());
    }

signals:
    void tabsLoaded(std::vector<TabInfo> tabs);
    void tabCreated(TabInfo tab);
    void activeTabChanged(TabId id);
    void navigationRequested(NavigationRequestedArgs args);
    void titleChanged(TabTitleChangedArgs args);
    void loadingStatusChanged(TabLoadingStatusChangedArgs args);
    void loadingProgressChanged(TabLoadingProgressChangedArgs args);

public slots:
    void loadTabs();
    void createTab(QUrl url);
    void createTab();
    void closeTab(TabId id);
    void changeActiveTab(TabId id);
    void moveTab(TabId id, int newIndex);
    void goForward(TabId id);
    void goBack(TabId id);
    void visitUrl(TabId id, QUrl url);
    void changeTabUrl(TabId id, QUrl url);
    void changeTabTitle(TabId id, QString title);
    void changeTabLoadingProgress(TabId id, int progress);
    void setTabLoadingStatus(TabId id, bool isLoading);

    void reloadTab(TabId id);

private:
    void _setupEvents();
    std::vector<std::unique_ptr<ISubscription>> _subscriptions;
    BrowserCore *_core;
};
#endif
