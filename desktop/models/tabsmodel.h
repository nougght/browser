#ifndef TABSMODEL_H
#define TABSMODEL_H

#include <QAbstractListModel>
#include <core/models.h>

class TabsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {UrlRole = Qt::UserRole + 1,
                 BackNavigatingRole,
                 ForwardNavigatingRole,
                 IsLoadingRole};
    explicit TabsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return _tabs.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addTab(const TabInfo &tab);
    void updateTabTitle(TabId id, std::string title);
    void updateTabUrl(TabId id, Url url);
    void updateTabLoading(TabId id, bool isLoading);
    void updateTabNavigation(TabId id, bool canGoBack, bool canGoForward);

    void setInitialTabs(std::vector<TabInfo> tabs);

    TabId getTabIdByIndex(int index)
    {
        return _tabs[index].id;
    }

    int getTabIndex(TabId id);
    TabInfo getTabInfo(TabId id);
private:

    std::vector<TabInfo> _tabs;
};

#endif // TABSMODEL_H
