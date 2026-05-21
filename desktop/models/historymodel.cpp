#include "historymodel.h"

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

    return QVariant();
}


void HistoryModel::addEntry(const HistoryEntry &entry)
{
    beginInsertRows(QModelIndex(), 0, 0);
    _history.insert(_history.begin(), entry);
    endInsertRows();
    qDebug() << "\n entry added: " << _history.size() << "\n";
}


void HistoryModel::addEntries(std::vector<HistoryEntry> &entries)
{
    beginInsertRows(QModelIndex(), 0, 0);
    _history.insert(_history.begin(), entries.begin(), entries.end());
    endInsertRows();
    qDebug() << "\n entries added: " << _history.size() << "\n";
}
