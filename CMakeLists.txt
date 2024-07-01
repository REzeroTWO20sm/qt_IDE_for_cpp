cmake_minimum_required(VERSION 3.14)

project(C_2plus_syntax_highlighting LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core LinguistTools)

set(TS_FILES C_2plus_syntax_highlighting_en_US.ts)

add_executable(C_2plus_syntax_highlighting
  main.cpp
  ${TS_FILES}
  syntaxHighlighter.h
  syntaxHighlighter.cpp
  res.qrc
  save_and_build.h
  save_and_build.cpp
)
target_link_libraries(C_2plus_syntax_highlighting PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

include(GNUInstallDirs)
install(TARGETS C_2plus_syntax_highlighting
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
