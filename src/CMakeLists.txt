set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/VehicleViewer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VehicleViewer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegulationKernelWrapper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegulationKernelWrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StdinReader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StdinReader.cpp
    PARENT_SCOPE)

set(EXECUTABLE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    PARENT_SCOPE)
set(FORMS
    ${CMAKE_CURRENT_SOURCE_DIR}/VehicleViewer.ui
    PARENT_SCOPE)
