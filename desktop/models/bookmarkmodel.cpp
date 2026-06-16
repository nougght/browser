#include "bookmarkmodel.h"

BookmarkModel::BookmarkModel(QObject *parent)
    : QAbstractListModel{parent}
{
    // _bookmarks = std::vector<Bookmark>{Bookmark{34, "a.kdfjdk.com", "dfjdkfdjdjfkd"}};
}


QVariant BookmarkModel::data(const QModelIndex &index, int role) const
{
    // qDebug() << "\nbookmark model data request\n";
    if (!index.isValid() || index.row() >= _bookmarks.size())
        return QVariant();

    const auto& b = _bookmarks[index.row()];

    if (role == Qt::DisplayRole)
        return QString::fromStdString(b.title);

    return QVariant();
}

std::optional<std::string> BookmarkModel::getBookmarkUrlByIndex(size_t index){
    if (index >= _bookmarks.size())
        return std::nullopt;
    return _bookmarks[index].url;
}

std::optional<int64_t> BookmarkModel::getBookmarkIdByIndex(size_t index){
    if (index >= _bookmarks.size())
        return std::nullopt;
    return _bookmarks[index].id;
}

std::optional<Bookmark> BookmarkModel::getBookmarkById(int64_t id){
    auto it = _idIndex.find(id);
    if (it == _idIndex.end())
        return std::nullopt;
    return _bookmarks[it->second];
}
std::optional<size_t> BookmarkModel::getBookmarkIndexById(int64_t id){
    auto it = _idIndex.find(id);
    if (it == _idIndex.end())
        return std::nullopt;
    return it->second;
}

void BookmarkModel::add(const Bookmark &bookmark){
    beginInsertRows(QModelIndex(), _bookmarks.size(), _bookmarks.size());
    _bookmarks.insert(_bookmarks.end(), bookmark);
    _urlIndex[bookmark.url] = _bookmarks.size() - 1;
    _idIndex[bookmark.id] = _bookmarks.size() - 1;
    endInsertRows();
    qDebug() << "\n bookmark added: size = " << _bookmarks.size() << "\nurl index size = " << _urlIndex.size() <<"\n";
}
void BookmarkModel::addAll(std::vector<Bookmark> &bookmarks)
{
    beginInsertRows(QModelIndex(), _bookmarks.size(), _bookmarks.size());
    size_t firstInd = _bookmarks.size();
    _bookmarks.insert(_bookmarks.end(), bookmarks.begin(), bookmarks.end());
    for (int i = 0; i < bookmarks.size(); ++i){
        _urlIndex[bookmarks[i].url] = firstInd + i;
        _idIndex[bookmarks[i].id] = firstInd + i;
    }
    endInsertRows();
    qDebug() << "\n bookmarks added: size = " << _bookmarks.size() << "\nurl index size = " << _urlIndex.size() <<"\n";
}

void BookmarkModel::remove(int64_t id)
{
    auto optionalInd = getBookmarkIndexById(id);
    if (!optionalInd.has_value()){
        qDebug() << "\n bookmark model remove bookmark not found: " << id << "\n";
        return;
    }
    auto index = optionalInd.value();
    beginRemoveRows(QModelIndex(), index, index);
    _deleteWithIndexUpdate(id, index);
    endRemoveRows();
}

void BookmarkModel::_deleteWithIndexUpdate(int64_t id, size_t ind)
{
    _idIndex.erase(id);
    _urlIndex.erase(_bookmarks[ind].url);
    _bookmarks.erase(_bookmarks.begin() + ind);
    for (auto i = ind; i < _bookmarks.size(); ++i){
        _urlIndex[_bookmarks[i].url] = i;
        _idIndex[_bookmarks[i].id] = i;
    }
}
bool BookmarkModel::isBookmarked(std::string url)
{
    bool isBookmarked = _urlIndex.count(url) > 0;
    qDebug() << "\nBM isBookmarked " << url << " = " << isBookmarked << "\n";
    return isBookmarked;
}
