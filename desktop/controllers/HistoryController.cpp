#include "HistoryController.h"

HistoryController::HistoryController(CoreAdapter *coreAdapter,
                                     HistoryModel *model)
    : _coreAdapter(coreAdapter), _historyModel(model) {
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
    _historyModel->addEntries(history);
    emit historyLoaded(std::move(history));
}

void HistoryController::onEntryAdded(HistoryEntry entry)
{
    qDebug() << "\n history controller entry added\n";
    _historyModel->addEntry(entry);
    emit entryAdded(entry);
}
