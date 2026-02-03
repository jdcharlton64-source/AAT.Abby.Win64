# CMakeList.txt : CMake project for AAT.Abby.DataAccess, include source and define
# project specific logic here.
#
# cmake_minimum_required(VERSION 4.2)

# Ensure DataAccess uses the same MSVC runtime as the rest of the solution

project(DataAccess VERSION 1.0.0)

# nlohmann json
set(NLohmannIncludePath ${CMAKE_CURRENT_LIST_DIR}\\third_party\\json\\single_include)
include_directories(${NLohmannIncludePath})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}\\third_party\\SQLiteCpp)

# Add source to this project's executable.
add_library(${PROJECT_NAME} STATIC "dbaccess.cpp")
# add_executable(${PROJECT_NAME} dbaccess.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# link SQLiteCpp and SQLite3
target_link_libraries(${PROJECT_NAME} PUBLIC SQLiteCpp sqlite3)