# Get Catch2
include(FetchContent)
FetchContent_Declare(Catch2
                     GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                     GIT_TAG v3.6.0)
FetchContent_MakeAvailable(Catch2)

# Create the target
file(GLOB_RECURSE MKVDB_TESTS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(mkvdb-tests ${MKVDB_TESTS_SOURCES})

# Link libraries
target_link_libraries(mkvdb-tests PRIVATE Catch2::Catch2WithMain
                                          mkvdb-common
                                          mkvdb-fs
                                          mkvdb-pager
                                          mkvdb-btree)
                                          
add_dependencies(mkvdb-tests mkvdb-common
                             mkvdb-fs
                             mkvdb-pager
                             mkvdb-btree)

# Configure CTest
include(CTest)
include(Catch)
catch_discover_tests(mkvdb-tests)

