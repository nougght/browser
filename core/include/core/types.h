#ifndef TYPES_H
#define TYPES_H
#include <string>

enum class SearchEngine {
    Google,
    Yandex,
    DuckDuckGo,
};

struct SearchSettings {
    SearchEngine engine;
    std::string baseUrl;
    std::string searchPath;
    std::string searchQueryParam;
};

// тип навигации в рамках одной вкладки
enum class NavigationType {
    NewPage,        // явный переход по новой ссылке
    Back,           // переход по истории назад
    Forward,        // переход по истории вперед
    Reload,         // перезагрузка страницы
    Redirect,       // редирект движка
    InternalPage    // внутренняя страница
};

#endif
