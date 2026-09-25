cmake_minimum_required(VERSION 3.16)


project(QTApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#include directories
# include_directories(includes)

#add_subdirectory(src)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)
find_package(nlohmann_json REQUIRED)

qt_standard_project_setup()



#add_executable(QTApp src/-)

qt_add_executable(QTApp
    src/Main.cpp
    src/Vehicle/Vehicle.cpp
    src/Vehicle/Canworker.cpp
    includes/Vehicle.h
    includes/Canworker.h
)

qt_add_qml_module(QTApp
    URI VehicleHMI
    VERSION 1.0

    QML_FILES
        qml/Main.qml
        qml/Settings.qml
)

# add_executable(QTApp
#     src/Vehicle/Vehicle.cpp
#     src/Main.cpp
#     includes/Vehicle.h
# )

target_include_directories(QTApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(QTApp PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick  nlohmann_json::nlohmann_json)