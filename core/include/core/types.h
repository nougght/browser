#ifndef TYPES_H
#define TYPES_H
#include <string>

enum class SearchEngine {
    Google,
    Yandex,
    DuckDuckGo,
    Count,
};

struct SearchSettings {
    SearchEngine engine;
    std::string baseUrl;
    std::string searchPath;
    std::string searchQueryParam;
};

// base urls

const std::string GoogleBaseUrl = "https://www.google.com";
const std::string YandexBaseUrl = "https://yandex.com";
const std::string DuckDuckGoBaseUrl = "https://duckduckgo.com";

// search paths

const std::string GoogleSearchPath = "/search";
const std::string YandexSearchPath = "/search";
const std::string DuckDuckGoSearchPath = "";

// search query params

const std::string GoogleSearchQueryParam = "q";
const std::string YandexSearchQueryParam = "text";
const std::string DuckDuckGoSearchQueryParam = "q";


std::string searchEngineToString(SearchEngine engine);

std::string searchEngineToBaseUrl(SearchEngine engine);
std::string searchEngineToSearchPath(SearchEngine engine);
std::string searchEngineToSearchQueryParam(SearchEngine engine);


// тип навигации в рамках одной вкладки
enum class NavigationType {
    NewPage,        // явный переход по новой ссылке
    Back,           // переход по истории назад
    Forward,        // переход по истории вперед
    BackForwardCompleted,
    Reload,         // перезагрузка страницы
    Redirect,       // редирект движка
    InternalPage    // внутренняя страница
};

#endif
