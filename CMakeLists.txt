cmake_minimum_required(VERSION 3.15)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(
    stm32f469-disco-test
    VERSION 0.1
    DESCRIPTION "Hello world for a STM32F469I Discovery board"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Test what this is
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE HAL_FILES REQUIRE_DEPENDS "src/hal/*.hpp" "src/hal/*.cpp")

message(NOTICE "List of all sources: ${HAL_FILES}")

add_library(hal_lib STATIC ${HAL_FILES})
set_target_properties(hal_lib PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB_RECURSE BSP_FILES REQUIRE_DEPENDS "src/bsp/*.hpp" "src/bsp/*.cpp")

message(NOTICE "List of all sources: ${BSP_FILES}")

add_library(bsp_lib STATIC ${BSP_FILES})
set_target_properties(bsp_lib PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(bsp_lib PRIVATE "src/bsp/include" "src")

#add_library(startup_lib STATIC startup.c)
#set_target_properties(startup_lib PROPERTIES LINKER_LANGUAGE ASM)

file(GLOB_RECURSE HELLOWORLD_FILES REQUIRE_DEPENDS "src/applications/hello-world/*.hpp" "src/applications/hello-world/*.cpp" "startup.c")

message(NOTICE "List of all sources: ${HELLOWORLD_FILES}")

add_executable(stm32f469-disco-test ${HELLOWORLD_FILES})

target_link_libraries( stm32f469-disco-test
                       hal_lib bsp_lib )

target_include_directories(stm32f469-disco-test PUBLIC "src")
set_target_properties(stm32f469-disco-test PROPERTIES OUTPUT_NAME "hello-world.elf")

target_compile_options(stm32f469-disco-test PRIVATE
                       $<$<COMPILE_LANG_AND_ID:CXX,GNU>:
                       -fno-rtti -fno-exceptions -std=c++20>)
target_compile_options(stm32f469-disco-test PRIVATE
                        $<$<COMPILE_LANG_AND_ID:C,GNU>:
                        -std=c99>)

target_compile_options(stm32f469-disco-test PRIVATE -gdwarf-2 -mthumb -mcpu=cortex-m4 -ffunction-sections -fdata-sections -DNO_SYSTEMINIT -Wall -Wextra -O0)


target_compile_options(bsp_lib PRIVATE
                       $<$<COMPILE_LANG_AND_ID:CXX,GNU>:
                       -fno-rtti -fno-exceptions -std=c++20>)
target_compile_options(bsp_lib PRIVATE
                        $<$<COMPILE_LANG_AND_ID:C,GNU>:
                        -std=c99>)

target_compile_options(bsp_lib PRIVATE -gdwarf-2 -mthumb -mcpu=cortex-m4 -ffunction-sections -fdata-sections -DNO_SYSTEMINIT -Wall -Wextra -O0)