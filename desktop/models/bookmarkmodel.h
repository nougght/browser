#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QAbstractListModel>
#include <core/models.h>
#include <QUrl>
#include <optional>

class BookmarkModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit BookmarkModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return _bookmarks.size();
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void add(const Bookmark &bookmark);
    void addAll(std::vector<Bookmark> &bookmarks);
    void remove(int64_t id);
    std::optional<std::string> getBookmarkUrlByIndex(size_t index);
    std::optional<int64_t> getBookmarkIdByIndex(size_t index);
    std::optional<Bookmark> getBookmarkById(int64_t id);
    std::optional<size_t> getBookmarkIndexById(int64_t id);
    bool isBookmarked(std::string url);

private:
    void _deleteWithIndexUpdate(int64_t id, size_t ind);
    std::unordered_map<std::string, size_t> _urlIndex;
    std::unordered_map<int64_t, size_t> _idIndex;
    std::vector<Bookmark> _bookmarks;
};

#endif // BOOKMARKMODEL_H
