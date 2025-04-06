cmake_minimum_required(VERSION 3.14)
project(ApeCore)

add_library(ApeCore STATIC
    ApeCore.cpp
)

target_include_directories(ApeCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(ApeCore PUBLIC cxx_std_17)
