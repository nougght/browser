#ifndef BOOKMARKDELEGATE_H
#define BOOKMARKDELEGATE_H

#include <QStyledItemDelegate>

class BookmarkDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BookmarkDelegate(QObject *parent = nullptr);
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
    void bookmarkClicked(const QModelIndex &index);
    void deleteClicked(const QModelIndex &index);
private:
    int _itemHMargin;
    int _itemVMargin;
};

#endif // BOOKMARKDELEGATE_H
