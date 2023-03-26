cmake_minimum_required(VERSION 3.26.1)
project(physics-engine LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME}
        src/sequential_acceleration_calculation.cpp
        src/openmp_acceleration_calculation.cpp
        src/opencl_acceleration_calculation.cpp
        src/acceleration_calculation_factory.cpp
        src/openmp_euler_position_velocity_calculation.cpp
        src/bodies_system.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include/)
# A CMake pattern to have headers that are not seen by the client of this library.
# So headers which not have to be visible for client-code should be placed in src/
target_include_directories(${PROJECT_NAME} PRIVATE src ${CMAKE_CURRENT_BINARY_DIR})

set(RESOURCES_FOLDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res/")
configure_file(config.h.in config.h @ONLY)

if (MSVC)
    # PRIVATE specifies that the specified compile options are only for the ${PROJECT_NAME} itself,
    # and not for any targets that link to it.
    # warning level 4 and all warnings as errors
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /EHsc /std:c++20)
else ()
    # PRIVATE specifies that the specified compile options are only for the ${PROJECT_NAME} itself,
    # and not for any targets that link to it.
    # lots of warnings and all warnings as errors
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()

# Add modules
add_subdirectory(lib/cpp-commons)
add_subdirectory(lib/opencl-toolkit)

# External libraries
# OpenMP
find_package(OpenMP REQUIRED)

target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX cpp-commons opencl-toolkit)

# Test environment
enable_testing()
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.13.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Unit tests as an executable of this library
add_executable(${PROJECT_NAME}-unit-tests
        test/unit/bodies_test.cpp
        test/unit/acceleration_calculation_factory_test.cpp
        test/unit/sequential_acceleration_calculation_test.cpp
        test/unit/openmp_acceleration_calculation_test.cpp
        test/unit/opencl_acceleration_calculation_test.cpp
        )

target_link_libraries(${PROJECT_NAME}-unit-tests gtest_main ${PROJECT_NAME} cpp-commons)