#ifndef UICONTEXT_H
#define UICONTEXT_H

#include <QObject>

#include "bookmarkmodel.h"
#include "historymodel.h"
#include "tabsmodel.h"

class UIContext : public QObject {
    Q_OBJECT
public:
    UIContext(QObject *parent = nullptr);

    TabsModel *getTabsModel();
    BookmarkModel *getBookmarkModel();
    HistoryModel *getHistoryModel();

    TabId activeTabId(){return _tabsModel->getActiveTabId();}
    void setActiveTabId(TabId id) {_tabsModel->setActiveTabId(id);}

    std::string activeTabURL(){
        return _tabsModel->getTabInfo(_tabsModel->getActiveTabId()).url.toStdString();
    }

private:
    TabsModel *_tabsModel;
    BookmarkModel *_bookmarkModel;
    HistoryModel *_historyModel;
};

#endif // UICONTEXT_H
