#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractListModel>
#include <core/models.h>

class HistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit HistoryModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return _history.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addEntry(const HistoryEntry &entry);
    void addEntries(std::vector<HistoryEntry> &entries);
    void removeEntry(int64_t id);
    void removeAll();
    std::optional<HistoryEntry> getEntryByIndex(int index);

private:
    std::optional<size_t> _getIndexById(int64_t id);
    void _addAllWithIndexUpdate(const std::vector<HistoryEntry> &entries);
    void _deleteWithIndexUpdate(int64_t id, size_t ind);
    std::vector<HistoryEntry> _history;
    std::unordered_map<int64_t, size_t> _idIndex;
};

#endif // HISTORYMODEL_H
