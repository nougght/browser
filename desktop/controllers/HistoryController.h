#ifndef HISTORY_CONTROLLER_H
#define HISTORY_CONTROLLER_H

#include <QObject>
#include <core/Identifier.h>
#include "adapter/CoreAdapter.h"
#include "models/historymodel.h"

class HistoryController : public QObject
{
    Q_OBJECT

public:
    HistoryController(CoreAdapter *coreAdapter, HistoryModel *historyModel);

    // slots for core signals
    void onHistoryLoaded(std::vector<HistoryEntry>);
    void onEntryAdded(HistoryEntry entry);


signals:

    void historyLoaded(std::vector<HistoryEntry> tabs);
    void entryAdded(HistoryEntry entry);


private:

    void _setupEvents();

    CoreAdapter *_coreAdapter;
    HistoryModel *_historyModel;
};

#endif
