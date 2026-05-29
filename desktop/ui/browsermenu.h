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
    void _onBookmarksClicked();
    void _onNewTabClicked();

    QAction *_newTabAct;
    QAction *_historyAct;
    QAction *_bookmarksAct;
public:
    BrowserMenu();
signals:
    void newTabClicked();
    void historyClicked();
    void bookmarksClicked();

};

#endif // BROWSERMENU_H
