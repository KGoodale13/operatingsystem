cmake_minimum_required(VERSION 3.8)
project(operatingsystem)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(SOURCE_FILES main.c kernel/opcodes.h kernel/opcodes.c kernel/memory.h kernel/memory.c loader/fileloader.h loader/fileloader.c kernel/executionContext.h kernel/executionContext.c kernel/processControlBlock.h kernel/processControlBlock.c util/util.c util/util.h kernel/scheduler.h kernel/scheduler.c)
add_executable(operatingsystem ${SOURCE_FILES})