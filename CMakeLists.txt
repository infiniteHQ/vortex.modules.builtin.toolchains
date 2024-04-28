cmake_minimum_required(VERSION 3.0)
project(module)
set(MODULE_TAG "vortex.modules.builtin.tasklists.1.1.0")

# YOU NEED TO INSTALL WITH THIS OPTION TURNED ON BEFORE COMPILING COMPLETELY !
option(DISABLE_MODULE "Disable module for recursive support" OFF)

if(DISABLE_MODULE)
add_definitions(-DDISABLE_MODULE)
endif()

# Include vortex library
find_library(VORTEX_SHARED_LIBRARY vortex_shared HINTS /lib)

# Vortex verificatioon
if(NOT VORTEX_SHARED_LIBRARY)
    message(FATAL_ERROR "Please build/install the vortex core before compiling modules!")
endif()

# Create module.so
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(module SHARED ${SOURCES})

# Compile & link
target_compile_options(module PRIVATE -Wall -Wextra)
target_link_libraries(module PRIVATE ${VORTEX_SHARED_LIBRARY})
target_include_directories(module PRIVATE /usr/include/Vortex)

set(MODULE_TAG "vortex.modules.builtin.toolchain.1.1.0")
set(INSTALL_INCLUDE_DIR /usr/local/include/Vortex/modules/${MODULE_TAG}/)

# Installation des fichiers d'en-tête de /main/include
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${INSTALL_INCLUDE_DIR}/
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.inl"
)