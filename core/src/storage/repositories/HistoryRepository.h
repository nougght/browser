#ifndef HISTORY_REPOSITORY_H
#define HISTORY_REPOSITORY_H

#include "interfaces/IHistoryRepository.h"
#include "core/interfaces/ICoreDispatcher.h"
#include "storage/DatabaseManager.h"

class HistoryRepository : public IHistoryRepository {
private:
    DatabaseManager *_dbManager;
    ICoreDispatcher *_dispatcher;
public:
    HistoryRepository(DatabaseManager *dbManager, ICoreDispatcher *dispatcher);

    void addVisit(HistoryEntry &entry) override;
    void deleteEntry(int64_t id) override;
    void deleteAll() override;

    void getHistory(std::function<void(std::vector<HistoryEntry>)>
                        callback) override;
};
#endif
