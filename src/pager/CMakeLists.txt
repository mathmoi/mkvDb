file(GLOB_RECURSE MKVDB_PAGER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(mkvdb-pager ${MKVDB_PAGER_SOURCES})

target_include_directories(mkvdb-pager PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(mkvdb-pager PRIVATE mkvdb-common)
add_dependencies(mkvdb-pager mkvdb-common)