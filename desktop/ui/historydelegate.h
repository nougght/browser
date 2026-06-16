#ifndef HISTORYDELEGATE_H
#define HISTORYDELEGATE_H


#include <QStyledItemDelegate>

// delegate for history item in history list
class HistoryDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit HistoryDelegate(QObject *parent = nullptr);
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
signals:
    void historyClicked(const QModelIndex &index);
    void deleteClicked(const QModelIndex &index);
private:
    int _rectHMargin;
    int _rectVMargin;
};

#endif // HISTORYDELEGATE_H
