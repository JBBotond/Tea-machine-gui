cmake_minimum_required(VERSION 3.16)
project(Tea-machine-gui VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    coinAcceptor.cpp
    coinAcceptor.h
    fsmworker.cpp
    fsmworker.h
    fsm/fsm_states.c
    fsm/events.c
    fsm/events.h
    fsm/states.c
    fsm/states.h
    fsm/fsm_functions/fsm.c
    fsm/fsm_functions/fsm.h
)
set_source_files_properties(
    fsm/fsm_states.c
    fsm/events.c
    fsm/states.c
    fsm/fsm_functions/fsm.c
    PROPERTIES LANGUAGE C
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Tea-machine-gui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Tea-machine-gui SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Tea-machine-gui
            ${PROJECT_SOURCES}
        )
    endif()
endif()
target_link_libraries(Tea-machine-gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(Tea-machine-gui PRIVATE fsm fsm/fsm_functions)
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Tea-machine-gui)
endif()
set_target_properties(Tea-machine-gui PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
include(GNUInstallDirs)
install(TARGETS Tea-machine-gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Tea-machine-gui)
endif()