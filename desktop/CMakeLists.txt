cmake_minimum_required(VERSION 3.16)

project(desktop VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets WebEngineWidgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets WebEngineWidgets LinguistTools)

set(TS_FILES desktop_ru_RU.ts)


set(PROJECT_SOURCES
        main.cpp
        ui/mainwindow.cpp
        ui/mainwindow.h
        ui/tabbarwithcontrol.h
        ui/tabbarwithcontrol.cpp

        adapter/CoreAdapter.h
        adapter/CoreAdapter.cpp

        controllers/MainWindowController.h
        controllers/MainWindowController.cpp
        controllers/TabsController.h
        controllers/TabsController.cpp
        controllers/HistoryController.h
        controllers/HistoryController.cpp

        controllers/BookmarkController.h

        models/tabsmodel.h
        models/tabsmodel.cpp

        binder/AppBinder.h
        resources.qrc
        ui/browsermenu.h ui/browsermenu.cpp
        ui/historypage.h ui/historypage.cpp
        models/historymodel.h models/historymodel.cpp
        models/bookmarkmodel.h models/bookmarkmodel.cpp
        ui/searchbar.h ui/searchbar.cpp
        controllers/BookmarkController.cpp
        ui/bookmarkspage.h ui/bookmarkspage.cpp

        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

qt_add_executable(desktop
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        models/UIContext.h models/UIContext.cpp
        ui/tabdelegate.h ui/tabdelegate.cpp



    )



qt_add_resources(desktop "resources.qrc")
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET desktop APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(desktop SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(desktop
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

add_subdirectory(../core core_build)
target_link_libraries(desktop PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
    browser_core
    )
target_include_directories(desktop PRIVATE
    ${CMAKE_SOURCE_DIR}
)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.desktop)
endif()
set_target_properties(desktop PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE FALSE

)

include(GNUInstallDirs)
install(TARGETS desktop
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(desktop)
endif()
