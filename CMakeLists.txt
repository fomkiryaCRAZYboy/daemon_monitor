cmake_minimum_required(VERSION 3.10)
project(daemon_monitor)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES
    audit_analysis.c
    "check&updt_daemons.c"
    init.c
    main.c
)

add_executable(monitor ${SOURCES})

set(MIPS_COMPILER "$ENV{HOME}/x-tools/mipsel-unknown-linux-gnu/bin/mipsel-unknown-linux-gnu-gcc")

if(EXISTS "${MIPS_COMPILER}")
    message(STATUS "Found MIPS compiler: ${MIPS_COMPILER}")
    
    file(WRITE ${CMAKE_BINARY_DIR}/mips-toolchain.cmake
        "set(CMAKE_SYSTEM_NAME Linux)\n"
        "set(CMAKE_SYSTEM_PROCESSOR mips)\n"
        "set(CMAKE_C_COMPILER ${MIPS_COMPILER})\n"
        "set(CMAKE_C_FLAGS \"-static\")\n"
        "set(CMAKE_EXE_LINKER_FLAGS \"-static\")\n"
    )
    
    add_custom_target(monitor-mips
        COMMAND ${CMAKE_COMMAND} 
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/mips-toolchain.cmake
            -B ${CMAKE_BINARY_DIR}/mips-build 
            -S ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/mips-build --target monitor
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${CMAKE_BINARY_DIR}/mips-build/monitor 
            ${CMAKE_BINARY_DIR}/monitor-mips
        COMMENT "Building MIPS version"
        VERBATIM
    )
else()
    message(WARNING "MIPS compiler not found at ${MIPS_COMPILER}")
endif()

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning build directory"
)

add_custom_target(show-pids
    COMMAND pstree --show-pids | grep "monitor" || true
    COMMENT "Showing monitor processes"
)