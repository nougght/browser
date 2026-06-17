#ifndef TABBARWITHCONTROL_H
#define TABBARWITHCONTROL_H

#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QStyledItemDelegate>
#include <qlabel.h>
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
    QPushButton *tabCountLabel() const{
        return _tabCountLabel;
    }
    QListView *tabsList() const{
        return _tabsList;
    }
    QPushButton *separator() const{
        return _separator;
    }
    QPushButton *addNewTabButton() const{
        return _addNewTabButton;
    }
    QPushButton *minimiseButton() const{
        return _minimiseButton;
    }
    QPushButton *maximizeButton() const{
        return _maximizeButton;
    }
    QPushButton *closeButton() const{
        return _closeButton;
    }

signals:
    void closeClicked();
    void maximizedChanged(bool maximized);
    void minimiseClicked();
    void newTabClicked();
    void tabClicked(int index);
    void tabCloseClicked(int index);
    void barDoubleClicked();
private:
    void resizeEvent(QResizeEvent *e) override;
    void setupUI();
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    // void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

    void onTabCloseClicked(const QModelIndex &index);
    void onTabClicked(const QModelIndex &index);
    void _updateTabsListWidth();
    void _setMaximized(bool maximized);
    void _onMaximizeClicked();

    const int _minTabListWidth = 105;
    int _tabListWidthLimit = -1;
    bool _isMaximized;
    bool _isSeparatorPressed = false;

    QAbstractListModel * _tabsModel;

    QHBoxLayout *_layout;
    QPushButton *_tabCountLabel;
    QListView *_tabsList;
    QPushButton *_separator;
    QFrame *_controlPanel;
    QPushButton *_addNewTabButton;
    QPushButton *_minimiseButton;
    QPushButton *_maximizeButton;
    QPushButton *_closeButton;
};

#endif // TABBARWITHCONTROL_H
