cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(snake)

set(TARGET snake)

add_executable(${TARGET})

target_sources(${TARGET} PRIVATE
    src/app.cpp
    src/game.cpp
    src/game.cpp
    src/menu.cpp
    )


target_include_directories(${TARGET} PRIVATE
    include
    )
