#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QFrame>
#include <QAbstractListModel>
#include <QListView>
#include <QVBoxLayout>
#include <QPushButton>

class HistoryPage : public QFrame
{
    Q_OBJECT
public:
    HistoryPage(QWidget *parent, QAbstractListModel *historyModel);
signals:
    void historyClicked(int index);
    void deleteClicked(int index);
    void clearClicked();
private:
    void setupUI();

    QAbstractListModel * _historyModel;

    QVBoxLayout *_vertLayout;
    QListView *_historyList;
    QPushButton *_clearButton;

    void onHistoryClicked(const QModelIndex &index);
    void onDeleteClicked(const QModelIndex &index);
    void onClearClicked();
};

#endif // HISTORYPAGE_H
