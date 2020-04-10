#cmake_minimum_required(VERSION 3.14)
cmake_minimum_required(VERSION 3.10.2)

# Project name
set(BOMBERMAN_PROJECT_NAME bomberman)
project(${BOMBERMAN_PROJECT_NAME} C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-g -Wall")
set(CMAKE_LD_FLAGS "-lpthread -lrt")

find_library(NCURSES_LIBRARY libncurses 
            NAMES libcurses.so 
                    libncurses.so
                    libncursesw.so
                    curses.so)
if (NOT NCURSES_LIBRARY)
    message(FATAL_ERROR "library not found")
endif()

list(APPEND CMAKE_LD_FLAGS "-lncurses")
set(GLOBAL_LIBS "${CMAKE_C_FLAGS} ${CMAKE_LD_FLAGS}")

# Include every src files
file(GLOB_RECURSE SRC_FILES "src/*.c" "src/*.h")

# Make executable
add_executable(${BOMBERMAN_PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${BOMBERMAN_PROJECT_NAME} ${GLOBAL_LIBS})
