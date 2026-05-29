#include "HistoryController.h"

HistoryController::HistoryController(CoreAdapter *coreAdapter,
                                     UIContext *ctx)
    : _coreAdapter(coreAdapter), _ctx(ctx) {
    _setupEvents();
}

void HistoryController::_setupEvents() {
    connect(_coreAdapter, &CoreAdapter::historyLoaded, this,
            &HistoryController::onHistoryLoaded);
    connect(_coreAdapter, &CoreAdapter::historyEntryAdded, this,
            &HistoryController::onEntryAdded);
}


void HistoryController::onHistoryLoaded(std::vector<HistoryEntry> history) {
    qDebug() << "\n history controller history loaded\n";
    _ctx->getHistoryModel()->addEntries(history);
    emit historyLoaded(std::move(history));
}

void HistoryController::onEntryAdded(HistoryEntry entry)
{
    qDebug() << "\n history controller entry added\n";
    _ctx->getHistoryModel()->addEntry(entry);
    emit entryAdded(entry);
}
