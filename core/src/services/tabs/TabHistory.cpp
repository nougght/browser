#include "domain/TabHistory.h"

void TabHistory::addUrl(Url newUrl)
{
    // если не на последнем элементе - очищаем последующие
    if (_currentIndex < _items.size() - 1)
    {
        _items.erase(_items.begin() + _currentIndex, _items.end());
    }
    _items.emplace_back(newUrl, "");
    _currentIndex = _items.size() - 1;

}

void TabHistory::goBack()
{
    if (_currentIndex >= 1)
    {
        --_currentIndex;
    }
}

void TabHistory::goForward()
{
    if (_currentIndex < _items.size() - 1)
    {
        ++_currentIndex;
    }
}

TabHistoryItem *TabHistory::currentItem()
{
    return _currentIndex == -1 ? nullptr : &_items[_currentIndex];
}

bool TabHistory::canGoBack()
{
    return _currentIndex >= 1;
}

bool TabHistory::canGoForward()
{
    return _currentIndex < _items.size() - 1;
}

void TabHistory::changeCurrentTitle(std::string newTitle)
{
    if (_currentIndex != -1)
    {
        _items[_currentIndex].title = newTitle;
    }
}
void TabHistory::changeCurrentUrl(Url url)
{
    if (_currentIndex != -1)
    {
        _items[_currentIndex].url = url;
    }
}
