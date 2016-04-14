cmake_minimum_required(VERSION 3.0.0)
project(bran)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_definitions(-std=c++11)

find_package(Qt5WebEngineWidgets REQUIRED)

add_executable(bran WIN32 bran.cpp requestinterceptor.cpp
	cookiejar.cpp view.cpp page.cpp browser.cpp downloadmanager.cpp
	dashboard.cpp mainwindow.cpp certificateerror.ui certificateerrorwidget.cpp
	resources.qrc)
target_link_libraries(bran Qt5::WebEngineWidgets)

if(WIN32 AND NOT CMAKE_CROSSCOMPILING)
	include(WinDeployQt)
	WinDeployQt(TARGET bran COMPILER_RUNTIME INCLUDE_MODULES ${QTLIBS})
endif()
