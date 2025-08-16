cmake_minimum_required(VERSION 3.5)
project(key_value_storage)

if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/main/include)
list(APPEND PATH_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/main/include)
endif()
if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/include)
list(APPEND PATH_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/test/include)
endif()
include_directories(${PATH_INCLUDES})

FILE (GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main/src/*)
FILE (GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main/src/* ${CMAKE_CURRENT_SOURCE_DIR}/test/src/*)

enable_testing()
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)
target_link_libraries(${PROJECT_NAME}_test gmock gtest gtest_main pthread)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD 20)

add_compile_options(
   -Werror

   -Wall
   -Wextra
   -Wpedantic

   -Wcast-align
   -Wcast-qual
   -Wconversion
   -Wctor-dtor-privacy
   -Wenum-compare
   -Wfloat-equal
   -Wnon-virtual-dtor
   -Wold-style-cast
   -Woverloaded-virtual
   -Wredundant-decls
   -Wsign-conversion
   -Wsign-promo
)

if(NOT CMAKE_CXX_EXTENSIONS)
   set(CMAKE_CXX_EXTENSIONS OFF)
endif()