#ifndef IBOOKMARK_REPOSITORY_H
#define IBOOKMARK_REPOSITORY_H

#include "core/models.h"

class IBookmarkRepository {
public:
    virtual void addBookmark(Bookmark &bookmark,std::function<void(Bookmark)> callback) = 0;
    virtual void deleteBookmark(int64_t id) = 0;

    virtual void getBookmarks(std::function<void(std::vector<Bookmark>)> callback) = 0;

};
#endif
