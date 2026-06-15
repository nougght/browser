#ifndef IBOOKMARK_REPOSITORY_H
#define IBOOKMARK_REPOSITORY_H

#include "core/models.h"
#include "domain/InternalErrors.h"
#include <functional>
#include <optional>

using BookmarkAddedCallback = std::function<void(std::optional<Bookmark>, RepositoryError)>;
using BookmarkDeletedCallback = std::function<void(RepositoryError)>;
using BookmarkGetCallback = std::function<void(std::vector<Bookmark>, RepositoryError)>;
class IBookmarkRepository {
public:
    virtual void addBookmark(Bookmark &bookmark, BookmarkAddedCallback callback) = 0;
    virtual void deleteBookmark(int64_t id, BookmarkDeletedCallback callback) = 0;

    virtual void getBookmarks(BookmarkGetCallback callback) = 0;

};
#endif
