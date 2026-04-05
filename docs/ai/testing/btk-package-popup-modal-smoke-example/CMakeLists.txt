cmake_minimum_required(VERSION 3.18)
if(POLICY CMP0177)
  cmake_policy(SET CMP0177 NEW)
endif()
project(btk_package_popup_modal_smoke LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BTK REQUIRED COMPONENTS Core Gui)
include("${BTK_CMAKE_DEPLOY_FILE}")

add_executable(btk_package_popup_modal_smoke main.cpp)
target_link_libraries(btk_package_popup_modal_smoke PRIVATE BTK::Gui BTK::Core)

install(TARGETS btk_package_popup_modal_smoke RUNTIME DESTINATION .)
btk_copy_library(CsCore .)
btk_copy_library(CsGui .)
btk_copy_plugins(CsGui .)
