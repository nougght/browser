#ifndef BOOKMARKSPAGE_H
#define BOOKMARKSPAGE_H

#include <QFrame>
#include <QAbstractListModel>
#include <QListView>
#include <QVBoxLayout>

// TODO: tree view for bookmarks
class BookmarksPage : public QFrame
{
    Q_OBJECT
public:
    BookmarksPage(QWidget *parent, QAbstractListModel *bookmarksModel);
signals:
    void bookmarkClicked(int index);
    void deleteClicked(int index);
private:
    void _setupUI();
    QAbstractListModel * _bookmarksModel;
    QVBoxLayout * _vertLayout;
    QListView * _bookmarksList;
    void onBookmarkClicked(const QModelIndex &index);
    void onDeleteClicked(const QModelIndex &index);
};

#endif // BOOKMARKSPAGE_H
