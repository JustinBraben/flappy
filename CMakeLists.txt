cmake_minimum_required(VERSION 3.16)

# Set the projectName variable
set(projectName "flappy")

project(${projectName} LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Fetch EnTT
FetchContent_Declare(EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.12.2)  # Specify the desired tag or commit
FetchContent_MakeAvailable(EnTT)

add_executable(${projectName} src/main.cpp)
target_link_libraries(${projectName} PRIVATE sfml-graphics EnTT::EnTT)
target_compile_features(${projectName} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${projectName}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${projectName}>
        VERBATIM)
endif()

install(TARGETS ${projectName})
