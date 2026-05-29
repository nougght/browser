#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QAbstractListModel>
#include <core/models.h>

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
    void remove(size_t index);
    std::string getUrl(size_t index);
    bool isBookmarked(std::string url);

private:
    std::unordered_map<std::string, size_t> _urlIndex;
    std::vector<Bookmark> _bookmarks;
};

#endif // BOOKMARKMODEL_H
