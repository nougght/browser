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


private:
    std::vector<HistoryEntry> _history;

};

#endif // HISTORYMODEL_H
