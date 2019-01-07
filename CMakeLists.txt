cmake_minimum_required(VERSION 3.0.4)
project(RTest
    VERSION 0.7.1)

set(BUILD_NUMBER "0" CACHE STRING "Build-Number")

set(VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}+${BUILD_NUMBER})

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
find_package(Qt5Xml)
find_package(Qt5Network)

add_subdirectory(src)

set(BUILD_SWIG TRUE CACHE BOOL "Enable building SWIG modules.")
add_subdirectory(RBase)

qt5_add_resources(RTEST_RSCS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/ressources/rtest.qrc)

add_library(rtest SHARED ${SRCS} ${RTEST_RSCS_SRCS})
add_library(rtests STATIC ${SRCS})
target_link_libraries(rtest Qt5::Widgets Qt5::Quick Qt5::QuickWidgets Qt5::Qml Qt5::Xml)
target_link_libraries(rtest rcomm)
target_link_libraries(rtest rregistry)
target_link_libraries(rtest rsupport)

target_link_libraries(rtests Qt5::Widgets Qt5::Quick Qt5::QuickWidgets Qt5::Qml Qt5::Xml)
target_link_libraries(rtests rcomm)
target_link_libraries(rtests rregistrys)
target_link_libraries(rtests rsupports)

add_executable(RTest ${EXECUTABLE_SRCS} ${FORMS_HPP} ${RTEST_RSCS_SRCS})
target_link_libraries(RTest rtests)

set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/qrc_rtest.cpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/qrc_rtest.cpp PROPERTY SKIP_AUTOGEN ON)

include(GNUInstallDirs)

install(TARGETS RTest
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static)
install(TARGETS rtest
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static)

# Install Python Libraries for RTest
install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/RBase/swig/python/RRegistry.py
		${CMAKE_CURRENT_BINARY_DIR}/RBase/swig/python/_RRegistry.so
		${CMAKE_CURRENT_BINARY_DIR}/RBase/swig/python/RSupport.py
		${CMAKE_CURRENT_BINARY_DIR}/RBase/swig/python/_RSupport.so
		DESTINATION share/RTest/python3
		)
# Install RBase shared libraries for python modules.
install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/RBase/RRegistry/librregistry.so
		${CMAKE_CURRENT_BINARY_DIR}/RBase/RSupport/librsupport.so
		DESTINATION share/RTest/
		)

set(CPACK_GENERATOR "DEB")

set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fully integrated regulation kernel test-tool for VERNER RKs.")
set(CPACK_PACKAGE_NAME "RTest")
set(CPACK_PACKAGE_VENDOR "Harptech")
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESSOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)
set(CPACK_RESSOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/packages)
set(CPACK_PACKAGE_CONTACT "Max Heisinger <mail@maximaximal.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "python3, python3-numpy, qml-module-qtquick-controls, qml-module-qtquick-layouts, qml-module-qtquick2, libqt5widgets5, libqt5gui5, libqt5quick5, libqt5qml5, libqt5quickwidgets5, swig, sqlite3")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/postinst;${CMAKE_CURRENT_SOURCE_DIR}/prerm")

include(CPack)
