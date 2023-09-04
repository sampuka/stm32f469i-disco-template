cmake_minimum_required(VERSION 3.15)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(
    stm32f469-disco-template
    VERSION 0.1
    DESCRIPTION "Hello world for a STM32F469I Discovery board"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Test what this is
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(stm32f469i-disco-bsp)

file(GLOB_RECURSE HELLOWORLD_FILES REQUIRE_DEPENDS "src/*.hpp" "src/*.cpp" "startup/startup.c")

message(NOTICE "List of all sources: ${HELLOWORLD_FILES}")

add_executable(stm32f469-disco-helloworld ${HELLOWORLD_FILES})

target_link_libraries(stm32f469-disco-helloworld stm32f469i-disco-bsp)

target_include_directories(stm32f469-disco-helloworld PUBLIC "src" "stm32f469i-disco-bsp/include")
set_target_properties(stm32f469-disco-helloworld PROPERTIES OUTPUT_NAME "hello-world.elf")

target_compile_options(stm32f469-disco-helloworld PRIVATE
                       $<$<COMPILE_LANG_AND_ID:CXX,GNU>:
                       -fno-rtti -fno-exceptions -std=c++20>)
target_compile_options(stm32f469-disco-helloworld PRIVATE
                        $<$<COMPILE_LANG_AND_ID:C,GNU>:
                        -std=c99>)

target_compile_options(stm32f469-disco-helloworld PRIVATE -gdwarf-2 -mthumb -mcpu=cortex-m4 -ffunction-sections -fdata-sections -DNO_SYSTEMINIT -Wall -Wextra -O0)