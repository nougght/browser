#ifndef TAB_H
#define TAB_H

// #include "TabId.h"

#include "TabHistory.h"
#include "Url.h"
#include "Identifier.h"
#include "Event.h"
#include "eventArgs.h"



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
    Tab(TabId id, Url url);
    TabId getId();
    Url getUrl();
    std::string getTitle();
    bool isLoading();
    int getLoadingProgress();

    TabInfo toTabInfo();

    void visitUrl(Url url);
    bool canGoBack();
    bool canGoForward();
    void goBack();
    void goForward();

    void changeTitle(std::string &title);
    void changeUrl(Url url);
    void changeLoadingProgress(int progress);
    void setLoadingStatus(bool isLoading);

    // Event<bool> canGoBackChanged;
    // Event<bool> canGoForwardChanged;
    // Event<TabInfo> wentBack;
    // Event<TabInfo> wentForward;
    Event<NavigationRequestedArgs> NavigationRequested;
    // Event<
    Event<Url> urlVisited;
    Event<Url> urlChanged;
    Event<std::string> titleChanged;
    Event<bool> loadingChanged;
};
#endif
