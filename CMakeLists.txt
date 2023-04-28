cmake_minimum_required(VERSION 3.5)

project(SerialReader VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools SerialPort REQUIRED)

set(TS_FILES SerialReader_ru_RU.ts)

file(GLOB cmd
    "src/*"
    "src/qdarkstyle/*/*"
)

set(BUILD_NUMBER 1)
if(NOT DEFINED BUILD_NUMBER)
    set(BUILD_NUMBER 9999)
endif()

set(COMPANY_NAME "НПП «ИТЭЛМА»")

add_executable(${PROJECT_NAME} ${cmd})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::SerialPort)

install(FILES ${QM_FILES} DESTINATION bin)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
