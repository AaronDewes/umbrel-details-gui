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

INCLUDE_DIRECTORIES(
    qrcode/
)

set(SRCS
    main.cpp
    window.cpp
    qrcode/QrCode.cpp
)

add_executable(umbrel-details ${SRCS})
target_link_libraries(umbrel-details Qt5::Core Qt5::Gui Qt5::Svg Qt5::Network)
