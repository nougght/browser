#ifndef TABBARWITHCONTROL_H
#define TABBARWITHCONTROL_H

#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QStyledItemDelegate>
#include "../models/tabsmodel.h"

class TabBarWithControl : public QFrame
{
    Q_OBJECT
public:
    TabBarWithControl(QWidget *parent, QAbstractListModel *model);
    void setupUI();
    // void addTab(const TabInfo &tab);
    // void updateTabTitle(TabId id, std::string title);
    // void updateTabUrl(TabId id, Url url);
    // void updateTabLoading(TabId id, bool isLoading);
    // void updateTabNavigation(TabId id, bool canGoBack, bool canGoForward);

    // void setInitialTabs(std::vector<TabInfo> tabs);


signals:
    void closeClicked();
    void minimiseClicked();
    void newTabClicked();
    void tabClicked(int index);
private:

    class ItemDelegate : public QStyledItemDelegate
    {
        QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override
        {
            return QSize(100, 30);
        }
    };

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
