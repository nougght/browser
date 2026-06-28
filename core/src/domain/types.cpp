#include <core/types.h>


std::string searchEngineToString(SearchEngine engine) {
    switch (engine) {
        case SearchEngine::Google:
            return "Google";
        case SearchEngine::Yandex:
            return "Yandex";
        case SearchEngine::DuckDuckGo:
            return "DuckDuckGo";
        default:
            return "";
    }
}

std::string searchEngineToBaseUrl(SearchEngine engine) {
    switch (engine) {
        case SearchEngine::Google:
            return GoogleBaseUrl;
        case SearchEngine::Yandex:
            return YandexBaseUrl;
        case SearchEngine::DuckDuckGo:
            return DuckDuckGoBaseUrl;
        default:
            return "";
    }
}

std::string searchEngineToSearchPath(SearchEngine engine) {
    switch (engine) {
        case SearchEngine::Google:
            return GoogleSearchPath;
        case SearchEngine::Yandex:
            return YandexSearchPath;
        case SearchEngine::DuckDuckGo:
            return DuckDuckGoSearchPath;
        default:
            return "";
    }
}

std::string searchEngineToSearchQueryParam(SearchEngine engine) {
    switch (engine) {
        case SearchEngine::Google:
            return GoogleSearchQueryParam;
        case SearchEngine::Yandex:
            return YandexSearchQueryParam;
        case SearchEngine::DuckDuckGo:
            return DuckDuckGoSearchQueryParam;
        default:
            return "";
    }
}