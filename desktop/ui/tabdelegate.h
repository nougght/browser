#ifndef TABDELEGATE_H
#define TABDELEGATE_H

#include <QStyledItemDelegate>

class TabDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TabDelegate(QObject *parent = nullptr);
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    bool editorEvent(QEvent* event,
                     QAbstractItemModel* model,
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) override;
signals:
    void tabClicked(const QModelIndex &index);
    void closeClicked(const QModelIndex &index);


};

#endif // TABDELEGATE_H
