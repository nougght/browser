#include "HistoryController.h"
#include <cstdint>

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
    connect(_coreAdapter, &CoreAdapter::historyEntryDeleted, this,
            &HistoryController::onEntryDeleted);
    connect(_coreAdapter, &CoreAdapter::historyCleared, this,
            &HistoryController::onHistoryCleared);
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

void HistoryController::onEntryDeleted(int64_t id) {
    qDebug() << "history controller entry deleted, id: " << id << "\n";
    _ctx->getHistoryModel()->removeEntry(id);
    // emit entryDeleted(id);
}

void HistoryController::onHistoryCleared() {
    qDebug() << "history controller history cleared\n";
    _ctx->getHistoryModel()->removeAll();
    // emit historyCleared();
}



void HistoryController::onEntryClicked(int index)
{
    auto optionalEntry = _ctx->getHistoryModel()->getEntryByIndex(index);
    if (!optionalEntry)
    {
        qDebug() << "history controller entry clicked not found: " << index;
        return;
    }
    _coreAdapter->createTab(QUrl(QString::fromStdString(optionalEntry.value().url)));
}

void HistoryController::onDeleteClicked(int index)
{
    auto optionalEntry = _ctx->getHistoryModel()->getEntryByIndex(index);
    if (!optionalEntry)
    {
        qDebug() << "history controller entry deleted not found: " << index;
        return;
    }
    _coreAdapter->deleteHistoryEntry(optionalEntry.value().id);
}

void HistoryController::onClearClicked()
{
    _coreAdapter->clearHistory();
}
