cmake_minimum_required(VERSION 3.18)
project(btk_package_platform_smoke LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BTK REQUIRED COMPONENTS Core Gui Network OpenGL Svg Sql Multimedia)

add_executable(btk_package_platform_smoke main.cpp)
target_link_libraries(btk_package_platform_smoke PRIVATE
   BTK::Multimedia
   BTK::Sql
   BTK::Svg
   BTK::OpenGL
   BTK::Network
   BTK::Gui
   BTK::Core
)
