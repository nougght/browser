
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
    src/domain/types.cpp
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
    src/services/bookmarks/BookmarkService.h src/services/bookmarks/BookmarkService.cpp
    src/storage/repositories/BookmarkRepository.h src/storage/repositories/BookmarkRepository.cpp
    src/util/util.cpp src/util/util.h


)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_library(browser_core STATIC
    ${CORE_SOURCES}
    src/storage/sql/init.sql

)

add_custom_command(TARGET browser_core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/src/storage/sql/init.sql
    $<TARGET_FILE_DIR:browser_core>
)
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/ada
    ${CMAKE_BINARY_DIR}/third_party/ada
    EXCLUDE_FROM_ALL
)

target_link_libraries(browser_core PRIVATE
    unofficial::sqlite3::sqlite3
    ada::ada
)

# target_include_directories, если только headers

target_include_directories(browser_core 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
