cmake_minimum_required(VERSION 3.0.0...3.13)

project(soloud)

find_package(Threads REQUIRED)

add_library(soloud)

add_compile_definitions(WITH_MINIAUDIO) # We are using Miniaudio as the back-end

set(BACKEND_SOURCE src/backend/miniaudio/soloud_miniaudio.cpp)
file(GLOB_RECURSE AUDIO_SOURCE src/audiosource/*.c*)
file(GLOB_RECURSE CORE src/core/*.c*)
file(GLOB_RECURSE FILTER src/filter/*.c*)

set(SOLOUD_SOURCES
    ${BACKEND_SOURCE}
    ${AUDIO_SOURCE}
    ${CORE}
    ${FILTER}
)

target_sources(soloud PRIVATE ${SOLOUD_SOURCES})
target_include_directories(soloud PUBLIC include)
target_link_libraries(soloud PRIVATE ${CMAKE_dL_LIBS} Threads::Threads)