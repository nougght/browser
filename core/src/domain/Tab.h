#ifndef TAB_H
#define TAB_H

// #include "TabId.h"

#include "TabHistory.h"
#include "core/Identifier.h"
#include <core/Event.h>
#include <core/Url.h>
#include <core/eventArgs.h>

// browser tab state with visit history
class Tab {
private:
    TabId _id;
    TabHistory _history;
    Url _url;
    std::string _title;
    bool _isLoading;
    int _loadingProgress;

public:
    explicit Tab(TabId id) : _id(id) {}
    Tab(TabId id, Url url, std::string title = "");
    TabId getId();
    Url getUrl();
    std::string getTitle();
    bool isLoading();
    int getLoadingProgress();
    bool isInternal() { return _url.isInternal(); }
    std::optional<InternalPageType> getInternalPageType() {
        return _url.getInternalPageType();
    }

    TabInfo toTabInfo();

    void visitUrl(Url url);
    bool canGoBack();
    bool canGoForward();
    void goBack();
    void goForward();

    void changeTitle(const std::string &title);
    void changeUrl(Url url);
    void changeLoadingProgress(int progress);
    void setLoadingStatus(bool isLoading);

    // Event<bool> canGoBackChanged;
    // Event<bool> canGoForwardChanged;
    // Event<TabInfo> wentBack;
    // Event<TabInfo> wentForward;
    // Event<NavigationRequestedArgs> NavigationRequested;
    
    Event<Url> urlVisited;
    Event<Url> urlChanged;
    Event<std::string> titleChanged;
    Event<bool> loadingChanged;
};
#endif
