cmake_minimum_required(VERSION 3.12)

PROJECT("sqlite-build")
include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT XCODE AND NOT MSVC AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_library(sqlite OBJECT sqlite3.c sqlite3.h sqlite3ext.h)


set(SQLITE_ABI sqlite-abi)

add_library(${SQLITE_ABI} SHARED json.hpp sqlite-abi.h sqlite-abi.cpp)
target_include_directories(${SQLITE_ABI} PUBLIC .)

target_compile_definitions(${SQLITE_ABI} PRIVATE SQLITE_ABI_SHARED SQLITE_ABI_BUILD)
set_target_properties(sqlite PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${SQLITE_ABI} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${SQLITE_ABI} PUBLIC sqlite ${CMAKE_THREAD_LIBS_INIT})

target_compile_definitions(
        sqlite PUBLIC
        SQLITE_ENABLE_RTREE
        SQLITE_ENABLE_FTS4
        SQLITE_ENABLE_FTS5
        SQLITE_ENABLE_JSON1
        SQLITE_ENABLE_RBU
        SQLITE_ENABLE_STAT4
)

if (WIN32 AND NOT MINGW32)
    target_compile_definitions(sqlite PUBLIC SQLITE_API=__declspec\(dllexport\))
else ()
    target_compile_definitions(sqlite PUBLIC SQLITE_API=__attribute__\(\(visibility\(\"default\"\)\)\))
endif ()