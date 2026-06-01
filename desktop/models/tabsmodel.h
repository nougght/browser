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
                 IsLoadingRole,
                 IsActiveRole};
    explicit TabsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return _tabs.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addTab(const TabInfo &tab);
    void removeTab(TabId id);
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
    void setActiveTabId(TabId id) {_activeTabId = id;}
    TabId getActiveTabId() {return _activeTabId;}
    TabInfo getTabInfo(TabId id);

signals:
    void tabAdded(QModelIndex *index);

private:
    std::vector<TabInfo> _tabs;
    std::unordered_map<TabId, size_t> _idIndex;
    TabId _activeTabId;
};

#endif // TABSMODEL_H
