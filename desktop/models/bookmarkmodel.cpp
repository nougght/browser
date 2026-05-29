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

std::string BookmarkModel::getUrl(size_t index){
    if (index >= _bookmarks.size())
        return "";
    return _bookmarks[index].url;
}

void BookmarkModel::add(const Bookmark &bookmark){
    beginInsertRows(QModelIndex(), _bookmarks.size(), _bookmarks.size());
    _bookmarks.insert(_bookmarks.end(), bookmark);
    _urlIndex[bookmark.url] = _bookmarks.size() - 1;
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
    }
    endInsertRows();
    qDebug() << "\n bookmarks added: size = " << _bookmarks.size() << "\nurl index size = " << _urlIndex.size() <<"\n";
}

void BookmarkModel::remove(size_t index)
{
    beginRemoveRows(QModelIndex(), index, index);
    _urlIndex.erase(_bookmarks[index].url);
    _bookmarks.erase(_bookmarks.begin() + index);
    endRemoveRows();
}

bool BookmarkModel::isBookmarked(std::string url)
{
    bool isBookmarked = _urlIndex.count(url) > 0;
    qDebug() << "\nBM isBookmarked " << url << " = " << isBookmarked << "\n";
    return isBookmarked;
}
