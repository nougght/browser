#ifndef BOOKMARKSPAGE_H
#define BOOKMARKSPAGE_H

#include <QFrame>
#include <QAbstractListModel>
#include <QListView>
#include <QVBoxLayout>

class BookmarksPage : public QFrame
{
    Q_OBJECT
public:
    BookmarksPage(QWidget *parent, QAbstractListModel *bookmarksModel);
private:
    void _setupUI();
    QAbstractListModel * _bookmarksModel;
    QVBoxLayout * _vertLayout;
    QListView * _bookmarksList;
};

#endif // BOOKMARKSPAGE_H
