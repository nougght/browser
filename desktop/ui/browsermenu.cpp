#include "browsermenu.h"

BrowserMenu::BrowserMenu() {
    newTabAct = new QAction("Новая вкладка", this);
    newTabAct->setShortcuts(QKeySequence::New);
    connect(newTabAct, &QAction::triggered, this, &BrowserMenu::_onNewTabClicked);

    historyAct = new QAction("История", this);
    // historyAct->setShortcuts(QKeySequence::);
    connect(historyAct, &QAction::triggered, this, &BrowserMenu::_onHistoryClicked);


    addAction(newTabAct);
    addAction(historyAct);

}


void BrowserMenu::_onHistoryClicked(){
    qDebug() <<"history clicked";
    emit historyClicked();
}

void BrowserMenu::_onNewTabClicked(){
    qDebug() <<"new tab menu clicked";
    emit newTabClicked();
}
