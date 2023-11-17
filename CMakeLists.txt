cmake_minimum_required(VERSION 3.27)
project(snake LANGUAGES CXX)

# Replace 2 lines below with target_compile_features(cxx_std_17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(TARGET snake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(${TARGET})

target_sources(${TARGET} PRIVATE
    src/app.cpp
    src/game.cpp
    src/main.cpp
    src/menu.cpp
    )

target_include_directories(${TARGET} PRIVATE
    include
    )

target_link_libraries(${TARGET} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system)

file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR}/bin)

install(TARGETS ${TARGET})
