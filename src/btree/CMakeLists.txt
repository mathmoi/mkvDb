file(GLOB_RECURSE MKVDB_BTREE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(mkvdb-btree ${MKVDB_BTREE_SOURCES})

target_include_directories(mkvdb-btree PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(mkvdb-btree PRIVATE mkvdb-common
                                          mkvdb-pager)
add_dependencies(mkvdb-btree mkvdb-common
                             mkvdb-pager)