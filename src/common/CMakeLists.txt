file(GLOB_RECURSE MKVDB_COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(mkvdb-common ${MKVDB_COMMON_SOURCES})

target_include_directories(mkvdb-common  PUBLIC ${PROJECT_SOURCE_DIR}/include)