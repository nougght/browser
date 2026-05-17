
set(CORE_SOURCES
    include/core/BrowserCore.h
    include/core/Url.h
    include/core/types.h
    include/core/models.h
    include/core/eventArgs.h
    include/core/Event.h
    include/core/Identifier.h
    include/core/Tab.h
    include/core/TabHistory.h
    include/core/interfaces/ICoreDispatcher.h

    src/BrowserCore.cpp
    src/common/Url.cpp
    src/services/tabs/TabManager.cpp
    src/services/tabs/TabManager.h
    src/services/tabs/Tab.cpp
    src/services/tabs/TabHistory.cpp
    src/services/tabs/TabIdGenerator.cpp
    src/services/tabs/TabIdGenerator.h
    src/services/history/HistoryService.h
    src/services/history/HistoryService.cpp
)

add_library(browser_core STATIC
    ${CORE_SOURCES}
)

target_include_directories(browser_core 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
