#include "tabdelegate.h"
#include "models/tabsmodel.h"
#include <QMouseEvent>
#include <QPainterPath>
#include <qabstractitemview.h>
#include <qapplication.h>
#include <qdebug.h>
#include <qpainter.h>

TabDelegate::TabDelegate(QObject *parent) : QStyledItemDelegate{parent} {}

QSize TabDelegate::sizeHint(const QStyleOptionViewItem &option,
                            const QModelIndex &index) const {
    return QSize(100, 30);
}

void TabDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    // 2px margin
    auto rect = option.rect.adjusted(2, 0, -2, 0);
    auto p = QPainterPath();
    p.addRoundedRect(rect, 8.0, 8.0);
    QColor bcolor;
    QColor tcolor;
    // 20x20 icon at right side
    auto closeRect = rect.adjusted(70, 4, -6, -4);
    auto textLeft = rect.left() + 6;
    // 4px spacing
    QRect textRect = QRect(textLeft, 0, closeRect.left() - textLeft - 4, rect.height());
    // qDebug() << "tab height = " << option.rect.height();
    // qDebug() << "closeRect.width() = " << closeRect.width();
    // qDebug() << "closeRect.height() = " << closeRect.height();
    // qDebug() << "closeRect.left() = " << closeRect.left();
    // qDebug() << "closeRect.right() = " << closeRect.right();
    
    auto closeIcon = QIcon(":/assets/cross.svg");
    // if (option.state & QStyle::State_Selected) {
    if (index.data(TabsModel::IsActiveRole).toBool()) {
        bcolor = QColor(208, 224, 230);
        tcolor = Qt::black;
    } else {
        bcolor = QColor(186, 212, 221);
        tcolor = QColor(49, 49, 49);
    }

    painter->fillPath(p, bcolor);

    // painter->fillRect(closeRect , Qt::blue);
    // painter->fillRect(textRect, Qt::red);

    if (_closeHovered && _hoveredIndex == index) { 
        auto closePath = QPainterPath();
        closePath.addRoundedRect(closeRect, 8.0, 8.0);
        painter->fillPath(closePath, QColor(0, 0, 0, 100));
    } 
    closeIcon.paint(painter, closeRect);


    painter->setBrush(tcolor);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft,
                      index.data(Qt::DisplayRole).toString());

    painter->restore();
}

bool TabDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) {
    QRect closeRect = option.rect.adjusted(72, 4, -8, -4);
    auto view = qobject_cast<const QAbstractItemView *>(option.widget);
    if (event->type() == QEvent::MouseButtonRelease) {
        auto *mouse = static_cast<QMouseEvent *>(event);
        auto isActive = model->data(index, TabsModel::IsActiveRole).toBool();

        qDebug() << "tab click " << mouse->pos() << " isActive: " << isActive;
        if (closeRect.contains(mouse->pos())) {

            qDebug() << "tab close clicked";
            emit closeClicked(index);
            return true;
        } else if (!isActive) {
            qDebug() << "inactive tab clicked";
            emit tabClicked(index);
        }
    }
    if (event->type() == QEvent::MouseMove) {
        auto *mouse = static_cast<QMouseEvent *>(event);
        auto closeHovered = closeRect.contains(mouse->pos());
        if (closeHovered != _closeHovered || _hoveredIndex != index) {
            if (_hoveredIndex.isValid()) {
                view->viewport()->update(closeRect);
            }
            _closeHovered = closeHovered;
            _hoveredIndex = index;
            qDebug() << "tab hover";
            view->viewport()->update(closeRect);
        }
    }
    if (event->type() == QEvent::Leave) {
        if (_closeHovered) {
            _closeHovered = false;
            _hoveredIndex = QPersistentModelIndex();
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
