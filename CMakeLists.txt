cmake_minimum_required(VERSION 3.23)

project(ao-cooldown LANGUAGES C)

add_executable(ao-cooldown main.c)

add_executable(ao-cooldown-test test.c) 

add_dependencies(ao-cooldown-test ao-cooldown)

target_compile_definitions(ao-cooldown-test PRIVATE "EXECUTABLE_PATH=\"$<TARGET_FILE:ao-cooldown>\"")
