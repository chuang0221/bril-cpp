cmake_minimum_required(VERSION 3.10)
project(BrilCpp)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    src/buildBlocks.cpp
    src/instr2str.cpp
    src/buildCFG.cpp
    src/common.cpp
    src/logger.cpp
    src/config.cpp
    src/localValueNumbering.cpp
    src/deadCodeElimination.cpp
)

add_library(bril_common ${SOURCES})

target_link_libraries(bril_common PRIVATE nlohmann_json::nlohmann_json)

add_executable(build_blocks src/buildBlocksMain.cpp)
target_link_libraries(build_blocks PRIVATE bril_common nlohmann_json::nlohmann_json)

add_executable(build_cfg src/buildCFGMain.cpp)
target_link_libraries(build_cfg PRIVATE bril_common nlohmann_json::nlohmann_json)

add_executable(local_value_numbering src/localValueNumberingMain.cpp)
target_link_libraries(local_value_numbering PRIVATE bril_common nlohmann_json::nlohmann_json)

add_executable(dead_code_elimination src/deadCodeEliminationMain.cpp)
target_link_libraries(dead_code_elimination PRIVATE bril_common nlohmann_json::nlohmann_json)