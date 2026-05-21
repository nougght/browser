#ifndef MODELS_H
#define MODELS_H

#include "Identifier.h"
#include "Url.h"
#include <string>

// 
struct TabInfo {
    TabId id;
    Url url;
    std::string title;

    bool canGoBack;
    bool canGoForward;
    bool isLoading;
};

struct HistoryEntry {
    int64_t id;
    std::string url;
    std::string title;
    int64_t timestamp;
};

#endif
