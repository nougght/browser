#include "bookmarkdelegate.h"
#include <QMouseEvent>
#include <QPainterPath>
#include <qpainter.h>

BookmarkDelegate::BookmarkDelegate(QObject *parent)
    : QStyledItemDelegate{parent} {
    _itemHMargin = 0;
    _itemVMargin = 0;
}

QSize BookmarkDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const {
    return QSize(200, 30);
}

void BookmarkDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    auto rect = option.rect.adjusted(_itemHMargin, _itemVMargin, -_itemHMargin,
                                     -_itemVMargin);

    auto deleteRect = rect.adjusted(rect.right() - 22, _itemVMargin + 7,
                                    -_itemHMargin - 7, -_itemVMargin - 7);
    auto textLeft = rect.left() + 6;
    auto textRect = QRect(textLeft, rect.top(), deleteRect.left() - textLeft - 4,
                          rect.height() - _itemVMargin * 2);

    //   qDebug() << "history height = " << option.rect.height();
    //   qDebug() << "deleteRect.height() = " << deleteRect.height();
    //   qDebug() << "deleteRect.width() = " << deleteRect.width();
    //   qDebug() << "textRect.top() = " << textRect.top();
    //   qDebug() << "deleteRect.top() = " << deleteRect.top();
    painter->setBrush(QColor(49, 49, 49));
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft,
                      index.data(Qt::DisplayRole).toString());

    auto deleteIcon = QIcon(":/assets/trash.svg");
    deleteIcon.paint(painter, deleteRect);

    // painter->fillRect(deleteRect, QColor(0, 0, 0));
    // painter->fillRect(textRect, Qt::red);
    painter->restore();
}

bool BookmarkDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) {
    if (event->type() == QEvent::MouseButtonRelease) {
        auto *mouse = static_cast<QMouseEvent *>(event);
        auto deleteRect = option.rect.adjusted(
            option.rect.right() - 22 - _itemHMargin, _itemVMargin + 7,
            -_itemHMargin - 7, -_itemVMargin - 7);
        if (deleteRect.contains(mouse->pos())) {
            qDebug() << "delete clicked at " << mouse->pos();
            emit deleteClicked(index);
            return true;
        }
        qDebug() << "history clicked at " << mouse->pos();
        emit bookmarkClicked(index);
        return true;
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
