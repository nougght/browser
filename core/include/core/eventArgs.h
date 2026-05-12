#ifndef EVENT_ARGS_H
#define EVENT_ARGS_H

// #include "Identifier.h"
// #include "Url.h"
#include "Identifier.h"
#include "models.h"




struct NavigationRequestedArgs
{
    NavigationType type;
    TabInfo tabInfo;
};

struct TabMovedArgs
{
    TabId id;
    int fromIndex;
    int toIndex;
};

struct TabTitleChangedArgs
{
    TabId id;
    std::string newTitle;
};

struct TabIconChangedArgs
{
    //
};


struct TabLoadingStatusChangedArgs
{
    TabId id;
    bool isLoading;
};

struct TabLoadingProgressChangedArgs
{
    TabId id;
    int progress;
};



#endif
