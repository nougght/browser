#include "tabsmodel.h"
#include <QBrush>
#include <QUrl>

TabsModel::TabsModel(QObject *parent) : QAbstractListModel{parent}, _tabs() {}

QVariant TabsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= _tabs.size())
        return QVariant();

    const auto &tab = _tabs[index.row()];

    if (role == Qt::DisplayRole)
        return QString::fromStdString(tab.title);
    // return "-" + QString::number(tab.id.value) + "-";
    if (role == Roles::UrlRole)
        return QString::fromStdString(tab.url.toStdString());
    if (role == Roles::BackNavigatingRole)
        return tab.canGoBack;
    if (role == Roles::ForwardNavigatingRole)
        return tab.canGoForward;
    if (role == Roles::IsLoadingRole)
        return tab.isLoading;
    if (role == Roles::IsActiveRole)
        return tab.id == _activeTabId;
    return QVariant();
}

void TabsModel::setInitialTabs(std::vector<TabInfo> tabs) {
    beginResetModel();
    // присваиваем без копирования
    _tabs = std::move(tabs);
    for (auto i = 0; i < _tabs.size(); ++i){
        _idIndex[_tabs[i].id] = i;
    }
    endResetModel();
}

void TabsModel::addTab(const TabInfo &tab) {
    beginInsertRows(QModelIndex(), _tabs.size(), _tabs.size());
    _tabs.push_back(tab);
    _idIndex[tab.id] = _tabs.size() - 1;
    endInsertRows();
    QModelIndex index = this->index(_tabs.size(), 0);
    emit tabAdded(std::move(&index));
}

void TabsModel::removeTab(TabId id) {
    int ind = getTabIndex(id);
    if (ind == -1)
        qDebug() << "removeTab tab not found: " << id.value;
        return;
    beginRemoveRows(QModelIndex(), ind, ind);
    _deleteWithIndexUpdate(id, ind);
    endRemoveRows();
}


int TabsModel::getTabIndex(TabId id) {
    auto it = _idIndex.find(id);
    if (it == _idIndex.end())
        return -1;
    return it->second;
}

void TabsModel::_deleteWithIndexUpdate(TabId id, size_t ind) {
    _idIndex.erase(id);
    _tabs.erase(_tabs.begin() + ind);
    for (auto i = ind; i < _tabs.size(); ++i)
    {
        _idIndex[_tabs[i].id] = i;
    }
}

void TabsModel::updateTabUrl(TabId id, Url url) {
    int ind = getTabIndex(id);

    if (ind == -1)
        qDebug() << "updateTabUrl tab not found: " << id.value;
        return;
    _tabs[ind].url = url;
    emit dataChanged(index(ind), index(ind), {UrlRole});
}

void TabsModel::updateTabTitle(TabId id, std::string title) {
    int ind = getTabIndex(id);

    if (ind == -1)
        qDebug() << "updateTabTitle tab not found: " << id.value;
        return;
    _tabs[ind].title = title;
    emit dataChanged(index(ind), index(ind), {Qt::DisplayRole});
}

void TabsModel::updateTabLoading(TabId id, bool isLoading) {
    int ind = getTabIndex(id);

    if (ind == -1)
        qDebug() << "updateTabLoading tab not found: " << id.value;
        return;
    _tabs[ind].isLoading = isLoading;
    emit dataChanged(index(ind), index(ind), {IsLoadingRole});
}

void TabsModel::updateTabNavigation(TabId id, bool canGoBack,
                                    bool canGoForward) {
    int ind = getTabIndex(id);

    if (ind == -1)
        qDebug() << "updateTabNavigation tab not found: " << id.value;
        return;
    _tabs[ind].canGoBack = canGoBack;
    _tabs[ind].canGoForward = canGoForward;
    emit dataChanged(index(ind), index(ind),
                     {BackNavigatingRole, ForwardNavigatingRole});
}


TabInfo TabsModel::getTabInfo(TabId id) {
    int ind = getTabIndex(id);
    if (ind == -1)
        // TODO: error handling
        throw std::runtime_error("Tab with id " + std::to_string(id.value) +
                                 " not found");
    return _tabs[ind];
}
