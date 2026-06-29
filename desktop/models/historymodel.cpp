#include "historymodel.h"
#include "util/util.h"

HistoryModel::HistoryModel(QObject *parent)
    : QAbstractListModel{parent}, _history()
{}



QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _history.size())
        return QVariant();

    const auto& entry = _history[index.row()];

    if (role == Qt::DisplayRole)
        return QString::fromStdString(entry.title) + " " + QString::fromStdString(entry.url);
    if (role == HistoryRoles::TimeRole) {
        return formatTimeLocal(entry.visitTime);
    }
    return QVariant();
}

std::optional<HistoryEntry> HistoryModel::getEntryByIndex(int index)
{
    if (index < 0 || index >= _history.size())
        return std::nullopt;
    return std::make_optional(_history[index]);
}

std::optional<size_t> HistoryModel::_getIndexById(int64_t id)
{
    auto it = _idIndex.find(id);
    if (it == _idIndex.end())
        return std::nullopt;
    return it->second;
}

void HistoryModel::_addAllWithIndexUpdate(const std::vector<HistoryEntry> &entries)
{
    for (auto &item : _idIndex){
        ++item.second;
    }
    _history.insert(_history.begin(), entries.begin(), entries.end());
    for (size_t i = 0; i < entries.size(); ++i)
        _idIndex[entries[i].id] = i;
}
void HistoryModel::_deleteWithIndexUpdate(int64_t id, size_t ind)
{
    _idIndex.erase(id);
    _history.erase(_history.begin() + ind);
    for (auto i = ind; i < _history.size(); ++i)
        _idIndex[_history[i].id] = i;
}

void HistoryModel::addEntry(const HistoryEntry &entry)
{
    beginInsertRows(QModelIndex(), 0, 0);
    _addAllWithIndexUpdate({entry});
    endInsertRows();
    qDebug() << "\n entry added: " << _history.size() << "\n";
}


void HistoryModel::addEntries(std::vector<HistoryEntry> &entries)
{
    beginInsertRows(QModelIndex(), 0, 0);
    _addAllWithIndexUpdate(entries);
    endInsertRows();
    qDebug() << "\n entries added: " << _history.size() << "\n";
}

void HistoryModel::updateEntry(const HistoryEntry &entry) {
    auto ind = _getIndexById(entry.id);
    if (!ind.has_value()) {
        qDebug() << "updateEntry entry not found: " << entry.id;
        return;
    }
    if (!entry.title.empty() && entry.title != _history[ind.value()].title) {
        _history[ind.value()].title = entry.title;
    }
    if (!entry.url.empty() && entry.url != _history[ind.value()].url) {
        _history[ind.value()].url = entry.url;
    }
    emit dataChanged(index(ind.value()), index(ind.value()), {Qt::DisplayRole});
}

void HistoryModel::removeEntry(int64_t id)
{
    auto ind = _getIndexById(id);
    if (!ind.has_value())
    {
        qDebug() << "removeEntry entry not found: " << id;
        return;
    }
    beginRemoveRows(QModelIndex(), ind.value(), ind.value());
    _deleteWithIndexUpdate(id, ind.value());
    endRemoveRows();
    qDebug() << "entry removed: " << _history.size() << "\n";
}

void HistoryModel::removeAll()
{
    if (_history.size() == 0)
    {
        qDebug() << "history cleared: no entries to clear\n";
        return;
    }
    beginRemoveRows(QModelIndex(), 0, _history.size() - 1);
    _idIndex.clear();
    _history.clear();
    endRemoveRows();
    qDebug() << "history cleared: " << _history.size() << "\n";
}

