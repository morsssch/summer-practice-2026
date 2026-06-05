cmake_minimum_required(VERSION 3.16)
project(Platformer)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Platformer ${SOURCES})

target_include_directories(Platformer PRIVATE src)

target_link_libraries(Platformer
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

add_custom_command(TARGET Platformer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:Platformer>/assets
)