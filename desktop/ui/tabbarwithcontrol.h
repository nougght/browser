#ifndef TABBARWITHCONTROL_H
#define TABBARWITHCONTROL_H

#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QStyledItemDelegate>
#include "../models/tabsmodel.h"

class ItemDelegate : public QStyledItemDelegate
{
public:
    ItemDelegate(){
        this->setObjectName("tabBarItem");
    }
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override
    {
        return QSize(100, 30);
    }
};

// tabbar with control (-,x) buttons
class TabBarWithControl : public QFrame
{
    Q_OBJECT
public:
    TabBarWithControl(QWidget *parent, QAbstractListModel *model);
    void setTabSelected(int ind);



signals:
    void closeClicked();
    void minimiseClicked();
    void newTabClicked();
    void tabClicked(int index);
    void barDoubleClicked();
private:
    void setupUI();
    void mouseDoubleClickEvent(QMouseEvent *event) override;


    void onTabClicked(const QModelIndex &index);
    QAbstractListModel * _tabsModel;

    QHBoxLayout *_layout;
    QListView *_tabsList;
    QFrame *_controlPanel;
    QPushButton *_addNewTabButton;
    QPushButton *_minimiseButton;
    QPushButton *_closeButton;
};

#endif // TABBARWITHCONTROL_H
