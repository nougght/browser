#ifndef TYPES_H
#define TYPES_H




// тип навигации в рамках одной вкладки
enum class NavigationType {
    NewPage,        // явный переход по новой ссылке
    Back,    // переход по истории
    Forward,
    Reload,         // перезагрузка страницы
    Redirect        // редирект движка
};


#endif
