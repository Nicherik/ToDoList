cmake_minimum_required(VERSION 3.28)
project(ToDoList CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




add_executable(ToDoList main.cpp
        src/JsonTaskStorage.cpp
        src/TaskService.cpp
        src/Task.cpp
        src/TaskList.cpp)


add_executable(tests
        tests/TestJsonTaskStorage.cpp
        tests/TestTaskService.cpp
        tests/TestTask.cpp
        external/catch2/catch_amalgamated.cpp
        tests/TestTaskList.cpp
        src/Task.cpp
        src/JsonTaskStorage.cpp
        src/TaskService.cpp
        src/TaskList.cpp

)



add_definitions(-DPROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_include_directories(ToDoList PRIVATE ${CMAKE_SOURCE_DIR}/json
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/enums
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/interface
        ${CMAKE_SOURCE_DIR}/external/catch2)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/json
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/enums
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/interface
        ${CMAKE_SOURCE_DIR}/external/catch2)