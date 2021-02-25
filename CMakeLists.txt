cmake_minimum_required(VERSION 3.13)
project(lunarengine)

include(pmm.cmake)

list(APPEND VCPKG_IMPORTS glfw3 glad entt imgui doctest spdlog range-v3 cereal)

pmm(VCPKG REVISION 2020.11 REQUIRES ${VCPKG_IMPORTS})

set(CMAKE_CXX_STANDARD 20)

add_library(lunarengine
        src/core/application_base.cpp
        src/core/application_base.hpp
        src/core/state_machine.cpp
        src/core/state_machine.hpp
        src/core/static_errors.hpp
        src/core/state_base.hpp
        src/core/screen.cpp
        src/core/screen.hpp
)
target_include_directories(lunarengine PUBLIC src)
target_include_directories(lunarengine PUBLIC extern)

find_package(cereal CONFIG REQUIRED)
target_link_libraries(lunarengine PRIVATE cereal)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(lunarengine PRIVATE EnTT::EnTT)

find_package(glad CONFIG REQUIRED)
target_link_libraries(lunarengine PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(lunarengine PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(lunarengine PRIVATE imgui::imgui)

find_package(range-v3 CONFIG REQUIRED)
target_link_libraries(lunarengine PRIVATE range-v3 range-v3-meta range-v3::meta range-v3-concepts)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(lunarengine PRIVATE spdlog::spdlog)

add_executable(tests tests/state_machine.test.cpp tests/main.cpp tests/application_framework.test.cpp)
target_compile_features(tests PRIVATE cxx_std_20)

find_package(doctest CONFIG REQUIRED)
target_link_libraries(tests PRIVATE doctest::doctest)

target_include_directories(tests PRIVATE src)
target_link_libraries(tests PRIVATE lunarengine)
target_include_directories(tests PRIVATE extern)