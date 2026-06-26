#ifndef IHISTORY_REPOSITORY_H
#define IHISTORY_REPOSITORY_H

#include <optional>
#include "core/models.h"
#include "domain/InternalErrors.h"


using HistoryAddedCallback = std::function<void(std::optional<HistoryEntry>, RepositoryError)>;
using HistoryUpdateCallback = std::function<void(RepositoryError)>;
using HistoryDeletedCallback = std::function<void(RepositoryError)>;
using HistoryGetCallback = std::function<void(std::vector<HistoryEntry>, RepositoryError)>;

class IHistoryRepository {
public:
    virtual void addVisit(HistoryEntry &entry, HistoryAddedCallback callback) = 0;
    virtual void updateEntry(HistoryEntry &entry, HistoryUpdateCallback callback) = 0;
    virtual void deleteEntry(int64_t id, HistoryDeletedCallback callback) = 0;
    virtual void deleteAll(HistoryDeletedCallback callback) = 0;

    virtual void getHistory(HistoryGetCallback callback) = 0;

};
#endif
