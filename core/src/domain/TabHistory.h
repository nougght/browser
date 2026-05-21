#ifndef TAB_HISTORY_H
#define TAB_HISTORY_H

#include <string>
#include <vector>
#include <core/Url.h>

// tab's visited history item
struct TabHistoryItem{
    TabHistoryItem(Url url_, std::string title_) : url(url_), title(title_)
    {}
    Url url;
    std::string title;
};

// visited site history of tab
class TabHistory{
private:
    std::vector<TabHistoryItem> _items;
    int _currentIndex = -1;
public:
    void addUrl(Url url);
    void goBack();
    void goForward();
    TabHistoryItem * currentItem();
    bool canGoBack();
    bool canGoForward();
    void changeCurrentTitle(std::string title);
    void changeCurrentUrl(Url url);
};

#endif
