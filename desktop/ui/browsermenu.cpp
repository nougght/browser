#include "browsermenu.h"

BrowserMenu::BrowserMenu() {
    _newTabAct = new QAction("Новая вкладка", this);
    _newTabAct->setShortcuts(QKeySequence::New);
    connect(_newTabAct, &QAction::triggered, this, &BrowserMenu::_onNewTabClicked);

    _historyAct = new QAction("История", this);
    // historyAct->setShortcuts(QKeySequence::);
    connect(_historyAct, &QAction::triggered, this, &BrowserMenu::_onHistoryClicked);

    _bookmarksAct = new QAction("Закладки",this);
    connect(_bookmarksAct, &QAction::triggered, this, &BrowserMenu::_onBookmarksClicked);

    addAction(_newTabAct);
    addAction(_historyAct);
    addAction(_bookmarksAct);

}


void BrowserMenu::_onHistoryClicked(){
    qDebug() <<"history clicked";
    emit historyClicked();
}


void BrowserMenu::_onBookmarksClicked(){
    qDebug() <<"bookmarks clicked";
    emit bookmarksClicked();
}


void BrowserMenu::_onNewTabClicked(){
    qDebug() <<"new tab menu clicked";
    emit newTabClicked();
}
