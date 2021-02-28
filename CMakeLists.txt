cmake_minimum_required(VERSION 3.13)
project(lunarengine)

# Setup Pmm and get deps
include(pmm.cmake)
list(APPEND VCPKG_IMPORTS glfw3 glad entt imgui doctest spdlog range-v3 cereal)
pmm(VCPKG REVISION 2020.11 REQUIRES ${VCPKG_IMPORTS})

# Set CPP version
set(CMAKE_CXX_STANDARD 20)

#setup lunarengine
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

#link deps
target_include_directories(lunarengine PUBLIC src)
target_include_directories(lunarengine PUBLIC extern)

find_package(cereal CONFIG REQUIRED)
target_link_libraries(lunarengine PUBLIC cereal)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(lunarengine PUBLIC EnTT::EnTT)

find_package(glad CONFIG REQUIRED)
target_link_libraries(lunarengine PUBLIC glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(lunarengine PUBLIC glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(lunarengine PUBLIC imgui::imgui)

find_package(range-v3 CONFIG REQUIRED)
target_link_libraries(lunarengine PUBLIC range-v3 range-v3-meta range-v3::meta range-v3-concepts)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(lunarengine PUBLIC spdlog::spdlog)

add_executable(tests tests/state_machine.test.cpp tests/main.cpp tests/application_framework.test.cpp tests/offscreen_render.test.cpp tests/invoke_external.hpp)

# Setup coverage
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(tests INTERFACE
            -O0        # no optimization
            -g         # generate debug info
            --coverage # sets all required flags
            )

    target_link_options(tests INTERFACE --coverage)
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(lunarengine INTERFACE
            -O0        # no optimization
            -g         # generate debug info
            --coverage # sets all required flags
            )

    target_link_options(lunarengine INTERFACE --coverage)
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

target_compile_features(tests PRIVATE cxx_std_20)

find_package(doctest CONFIG REQUIRED)
target_link_libraries(tests PRIVATE doctest::doctest)

target_include_directories(tests PRIVATE src)
target_link_libraries(tests PRIVATE lunarengine)
target_include_directories(tests PRIVATE extern)

#make test
enable_testing()
add_test(unit-test tests --reporters=junit --out=test.xml)
