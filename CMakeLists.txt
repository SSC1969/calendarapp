cmake_minimum_required(VERSION 4.2)
project(CalenderApp)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB_RECURSE SOURCES src/*.cpp)

find_package(wxWidgets COMPONENTS core base REQUIRED)
find_package(SQLite3)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE wxWidgets::wxWidgets SQLite::SQLite3)

# install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
