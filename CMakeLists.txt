project(umbrel-details)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-Wall)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Network REQUIRED)

INCLUDE_DIRECTORIES(
    qrcode/
)

set(SRCS
    main.cpp
    window.cpp
    qrcode/QrCode.cpp
)

QT5_ADD_RESOURCES(RSCS umbrel-details.qrc)

add_executable(umbrel-details ${SRCS} ${RSCS})
qt5_use_modules(umbrel-details Core)
qt5_use_modules(umbrel-details Gui)
qt5_use_modules(umbrel-details Svg)
qt5_use_modules(umbrel-details Network)
