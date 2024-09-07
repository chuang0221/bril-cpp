cmake_minimum_required(VERSION 3.10)
project(BrilCpp)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Common library
set(COMMON_SOURCES
    src/common/common.cpp
    src/common/config.cpp
    src/common/instr2str.cpp
    src/common/logger.cpp
)

# Preprocess library
set(PREPROCESS_SOURCES
    src/preprocess/buildBlocks.cpp
    src/preprocess/buildCFG.cpp
)

# Optimization library
set(OPTIMIZATION_SOURCES
    src/optimization/localValueNumbering.cpp
    src/optimization/deadCodeElimination.cpp
)

add_library(bril_common ${COMMON_SOURCES})
add_library(bril_preprocess ${PREPROCESS_SOURCES})
add_library(bril_optimization ${OPTIMIZATION_SOURCES})

target_link_libraries(bril_common PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(bril_preprocess PRIVATE bril_common)
target_link_libraries(bril_optimization PRIVATE bril_common bril_preprocess)

# Executables
add_executable(build_blocks src/driver/buildBlocksMain.cpp)
target_link_libraries(build_blocks PRIVATE bril_common bril_preprocess nlohmann_json::nlohmann_json)

add_executable(build_cfg src/driver/buildCFGMain.cpp)
target_link_libraries(build_cfg PRIVATE bril_common bril_preprocess nlohmann_json::nlohmann_json)

add_executable(local_value_numbering src/driver/localValueNumberingMain.cpp)
target_link_libraries(local_value_numbering PRIVATE bril_common bril_preprocess bril_optimization nlohmann_json::nlohmann_json)

add_executable(dead_code_elimination src/driver/deadCodeEliminationMain.cpp)
target_link_libraries(dead_code_elimination PRIVATE bril_common bril_preprocess bril_optimization nlohmann_json::nlohmann_json)