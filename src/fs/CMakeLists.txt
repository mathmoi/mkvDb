file(GLOB_RECURSE MKVDB_FS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(mkvdb-fs ${MKVDB_FS_SOURCES})

target_include_directories(mkvdb-fs PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(mkvdb-fs PRIVATE mkvdb-common)
add_dependencies(mkvdb-fs mkvdb-common)