cmake_minimum_required(VERSION 3.10) # use cmake version 3.10 or higher

project(audio_pipeline)
set(CMAKE_CXX_STANDARD 17) # require C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON) # prevent from using older standard

add_executable(audio_pipeline src/main.cpp)
target_include_directories(audio_pipeline PRIVATE dependencies)
find_package(Threads REQUIRED)
target_link_libraries(audio_pipeline PRIVATE Threads::Threads)