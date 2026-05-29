
set(CORE_SOURCES
    include/core/types.h
    include/core/models.h
    include/core/eventArgs.h
    include/core/Url.h
    include/core/Identifier.h
    include/core/Event.h
    include/core/interfaces/IBrowserCore.h
    include/core/interfaces/ICoreDispatcher.h

    src/BrowserCore.h
    src/BrowserCore.cpp
    src/domain/Url.cpp
    src/domain/Tab.h

    src/domain/TabHistory.h
    src/interfaces/IHistoryRepository.h
    src/storage/DatabaseManager.h
    src/storage/DatabaseManager.cpp
    src/storage/repositories/HistoryRepository.h
    src/storage/repositories/HistoryRepository.cpp
    src/services/tabs/TabManager.cpp
    src/services/tabs/TabManager.h
    src/services/tabs/Tab.cpp
    src/services/tabs/TabHistory.cpp
    src/services/tabs/TabIdGenerator.cpp
    src/services/tabs/TabIdGenerator.h
    src/services/history/HistoryService.h
    src/services/history/HistoryService.cpp


)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_library(browser_core STATIC
    ${CORE_SOURCES}
    src/storage/sql/init.sql
    src/services/bookmarks/BookmarkService.h src/services/bookmarks/BookmarkService.cpp
    src/storage/repositories/BookmarkRepository.h src/storage/repositories/BookmarkRepository.cpp

)

add_custom_command(TARGET browser_core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/src/storage/sql/init.sql
    $<TARGET_FILE_DIR:browser_core>
)

target_link_libraries(browser_core PRIVATE
    unofficial::sqlite3::sqlite3
)

target_include_directories(browser_core 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
