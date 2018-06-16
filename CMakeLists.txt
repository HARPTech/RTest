cmake_minimum_required(VERSION 3.0.4)
project(RTest)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Quick)
find_package(Qt5QuickWidgets)
find_package(Qt5Qml)
find_package(Qt5Network)

add_subdirectory(src)

add_subdirectory(RBase)

qt5_add_resources(RTEST_RSCS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/ressources/rtest.qrc)

add_library(rtest SHARED ${SRCS} ${RTEST_RSCS_SRCS})
target_link_libraries(rtest Qt5::Widgets Qt5::Quick Qt5::QuickWidgets Qt5::Qml Qt5::Network)
target_link_libraries(rtest rcomm)
target_link_libraries(rtest rregistry)
target_link_libraries(rtest rsupport)

add_executable(RTest ${EXECUTABLE_SRCS} ${FORMS_HPP})
target_link_libraries(RTest rtest)
