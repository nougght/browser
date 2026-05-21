#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QFrame>
#include <QAbstractListModel>
#include <QListView>
#include <QVBoxLayout>

class HistoryPage : public QFrame
{
    Q_OBJECT
public:
    HistoryPage(QWidget *parent, QAbstractListModel *historyModel);

private:
    void setupUI();

    QAbstractListModel * _historyModel;

    QVBoxLayout *_vertLayout;
    QListView *_historyList;

};

#endif // HISTORYPAGE_H
