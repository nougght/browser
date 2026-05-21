#ifndef BROWSERMENU_H
#define BROWSERMENU_H

#include <QMenu>
#include <QObject>
#include <QAction>

class BrowserMenu : public QMenu
{
    Q_OBJECT
private:
    void _onHistoryClicked();
    void _onNewTabClicked();

    QAction *newTabAct;
    QAction *historyAct;
public:
    BrowserMenu();
signals:
    void newTabClicked();
    void historyClicked();

};

#endif // BROWSERMENU_H
