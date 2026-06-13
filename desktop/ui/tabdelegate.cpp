#include "tabdelegate.h"
#include "models/tabsmodel.h"
#include <QMouseEvent>
#include <QPainterPath>
#include <qabstractitemview.h>
#include <qapplication.h>
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
    auto rect = option.rect.adjusted(2, 0, -2, 0);
    auto p = QPainterPath();
    p.addRoundedRect(rect, 8.0, 8.0);
    QColor bcolor;
    QColor tcolor;
    auto closeRect = rect.adjusted(70, 4, -6, -4);
    auto closeIcon = QIcon(":/assets/cross.svg");
    if (option.state & QStyle::State_Selected) {
        bcolor = QColor(208, 224, 230);
        tcolor = Qt::black;
    } else {
        bcolor = QColor(186, 212, 221);
        tcolor = QColor(49, 49, 49);
    }

    painter->fillPath(p, bcolor);
    closeIcon.paint(painter, closeRect);
    QRect textRect = rect.adjusted(8, 0, -28, 0);
    painter->setBrush(tcolor);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft,
                      index.data(Qt::DisplayRole).toString());

    painter->restore();
}

bool TabDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) {
    if (event->type() == QEvent::MouseButtonRelease) {
        auto *mouse = static_cast<QMouseEvent *>(event);
        QRect closeRect = option.rect.adjusted(72, 4, -8, -4);
        auto isActive = model->data(index, TabsModel::IsActiveRole).toBool();

        qDebug() << "tab click " << mouse->pos() << " isActive: " << isActive;
        if (closeRect.contains(mouse->pos())) {
            auto view = qobject_cast<const QAbstractItemView *>(option.widget);

            qDebug() << "tab close clicked";
            emit closeClicked(index);
            return true; // событие обработано
        } else if (!isActive) {
            qDebug() << "inactive tab clicked";
            emit tabClicked(index);
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
