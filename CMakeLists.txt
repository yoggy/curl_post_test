cmake_minimum_required(VERSION 2.8)

project(curl_post_test)

set(src_curl_post_test main_curl_post_test.cpp)

set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_C_FLAGS "-g")

find_library( CURL_LIBRARY NAMES curl curllib libcurl_imp curllib_static)

add_executable( curl_post_test ${src_curl_post_test} )
include_directories( /usr/local/include ${CURL_LIBRARY_DIR} )
target_link_libraries( curl_post_test pthread ${CURL_LIBRARY} )



