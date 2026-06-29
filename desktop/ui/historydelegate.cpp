#include "historydelegate.h"
#include <QMouseEvent>
#include <QPainterPath>
#include <qpainter.h>
#include "models/historymodel.h"

HistoryDelegate::HistoryDelegate(QObject *parent) : QStyledItemDelegate{parent}
{
    _rectHMargin = 4;
    _rectVMargin = 4;
    _timeWidth = 100;
}

QSize HistoryDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(200, 30);
}
void HistoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    auto rect = option.rect.adjusted(_rectHMargin, _rectVMargin, -_rectHMargin, -_rectVMargin);

    qDebug() << "rect height: " << rect.height();

    auto deleteRect = rect.adjusted(rect.width() - 16 - 6, 3, -6, -3);
    auto timeRect = QRect(deleteRect.left() - _timeWidth - 3, rect.top(), _timeWidth, rect.height());

    auto textLeft = rect.left() + 6;
    auto textRect = QRect(textLeft, rect.top(), timeRect.left() - textLeft - 3, rect.height());
    
    // painter->fillRect(rect, Qt::blue);
    // painter->fillRect(textRect, Qt::red);
    //   qDebug() << "history height = " << option.rect.height();
    // qDebug() << "deleteRect.height() = " << deleteRect.height();
    // qDebug() << "deleteRect.width() = " << deleteRect.width();
    // qDebug() << "timeRect.width() = " << timeRect.width();
    // qDebug() << "timeRect.height() = " << timeRect.height();
    //   qDebug() << "textRect.top() = " << textRect.top();
    //   qDebug() << "deleteRect.top() = " << deleteRect.top();

    painter->setBrush(QColor(49, 49, 49));
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, index.data(Qt::DisplayRole).toString());
    painter->drawText(timeRect, Qt::AlignVCenter | Qt::AlignRight, index.data(HistoryRoles::TimeRole).toString());
    auto deleteIcon = QIcon(":/assets/trash.svg");
    deleteIcon.paint(painter, deleteRect);

    painter->restore();
}

bool HistoryDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                                  const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        auto *mouse = static_cast<QMouseEvent *>(event);
        auto deleteRect = option.rect.adjusted(option.rect.width() - 16 - _rectHMargin - 6, _rectVMargin + 3,
                                               -_rectHMargin - 6, -_rectVMargin - 3);
        if (deleteRect.contains(mouse->pos()))
        {
            qDebug() << "delete clicked at " << mouse->pos();
            emit deleteClicked(index);
            return true;
        }
        qDebug() << "history clicked at " << mouse->pos();
        emit historyClicked(index);
        return true;
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
