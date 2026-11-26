cmake_minimum_required(VERSION 3.28)
project(anpai LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SFML_DIR "D:/sfml/SFML-3.1.0/lib/cmake/SFML")
find_package(SFML 3.1.0 EXACT COMPONENTS Graphics Window System REQUIRED)

file(GLOB_RECURSE PROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp")

add_executable(main main.cpp ${PROJECT_HEADERS})

target_include_directories(main PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")
target_link_libraries(main PRIVATE SFML::Graphics SFML::Window SFML::System)
