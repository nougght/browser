#ifndef HISTORY_REPOSITORY_H
#define HISTORY_REPOSITORY_H

#include "interfaces/IHistoryRepository.h"
#include "storage/DatabaseManager.h"

class HistoryRepository : public IHistoryRepository {
private:
    DatabaseManager *_dbManager;

public:
    HistoryRepository(DatabaseManager *dbManager);

    void addVisit(const HistoryEntry &entry) override;
    // void deleteEntry(int64_t id) override;
    // void deleteAll() override;

    // void getHistory(std::function<void(std::vector<HistoryEntry>)>
    //                     callback) override;
};
#endif
