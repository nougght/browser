#ifndef IHISTORY_REPOSITORY_H
#define IHISTORY_REPOSITORY_H

#include "core/models.h"

class IHistoryRepository {
public:
    virtual void addVisit(HistoryEntry &entry) = 0;
    // virtual void deleteEntry(int64_t id) = 0;
    // virtual void deleteAll() = 0;

    virtual void getHistory(std::function<void(std::vector<HistoryEntry>)> callback) = 0;

};
#endif
