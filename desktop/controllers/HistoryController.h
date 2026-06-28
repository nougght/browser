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
    void onEntryUpdated(HistoryEntry entry);
    void onEntryDeleted(int64_t id);
    void onHistoryCleared();

    // slots for ui signals
    void onEntryClicked(int index);
    void onDeleteClicked(int index);
    void onClearClicked();
signals:
    void historyLoaded(std::vector<HistoryEntry> history);
    void entryAdded(HistoryEntry entry);

    // unused
    void entryUpdated(HistoryEntry entry);
    void entryDeleted(int64_t id);
    void historyCleared();

private:

    void _setupEvents();

    CoreAdapter *_coreAdapter;
    UIContext *_ctx;
};

#endif
