cmake_minimum_required(VERSION 3.18)
project(btk_package_multimedia_smoke LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BTK REQUIRED COMPONENTS Core Multimedia)

add_executable(btk_package_multimedia_smoke main.cpp)
target_link_libraries(btk_package_multimedia_smoke PRIVATE BTK::Multimedia BTK::Core)
