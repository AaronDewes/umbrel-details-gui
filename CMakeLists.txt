project(umbrel-details)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-Wall)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5VirtualKeyboard REQUIRED)
find_package(Qt5WebEngine REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

INCLUDE_DIRECTORIES(
    qrcode/
)

set(SRCS
    main.cpp
    MainWidget.cpp
    window.cpp
    qrcode/QrCode.cpp
)

QT5_ADD_RESOURCES(RSCS umbrel-details.qrc)

QT5_WRAP_CPP(MOCS MainWidget.h)

add_executable(umbrel-details ${SRCS} ${RSCS} ${MOCS})
target_link_libraries(umbrel-details Qt5::Core Qt5::Gui Qt5::Svg Qt5::Network Qt5::VirtualKeyboard Qt5::WebEngine Qt5::WebEngineWidgets)
