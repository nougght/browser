#include "UIContext.h"

UIContext::UIContext(QObject *parent)
    : QObject{parent} {
    _tabsModel = new TabsModel();
    _historyModel = new HistoryModel();
    _bookmarkModel = new BookmarkModel();
}

TabsModel *UIContext::getTabsModel() { return _tabsModel; }
BookmarkModel *UIContext::getBookmarkModel() { return _bookmarkModel; }
HistoryModel *UIContext::getHistoryModel() { return _historyModel; }

