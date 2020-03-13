cmake_minimum_required (VERSION 3.8)

add_executable (CurlTest "main.cpp")

find_package(CURL CONFIG REQUIRED)
target_link_libraries(CurlTest PRIVATE CURL::libcurl)
