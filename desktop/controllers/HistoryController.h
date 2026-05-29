#ifndef HISTORY_CONTROLLER_H
#define HISTORY_CONTROLLER_H

#include <QObject>
#include <core/Identifier.h>
#include "adapter/CoreAdapter.h"
#include "models/historymodel.h"
#include "models/UIContext.h"

class HistoryController : public QObject
{
    Q_OBJECT

public:
    HistoryController(CoreAdapter *coreAdapter, UIContext *ctx);

    // slots for core signals
    void onHistoryLoaded(std::vector<HistoryEntry>);
    void onEntryAdded(HistoryEntry entry);


signals:

    void historyLoaded(std::vector<HistoryEntry> history);
    void entryAdded(HistoryEntry entry);


private:

    void _setupEvents();

    CoreAdapter *_coreAdapter;
    UIContext *_ctx;
};

#endif
