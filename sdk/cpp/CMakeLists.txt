cmake_minimum_required(VERSION 2.8)
project(RhobanSDK)

# Options
option(SDK
    "Enable SDK compiling" ON)

option(TESTS
    "Enable tests" OFF)

# Useful for getting common compiling
set(UTILS_DIRECTORY "${PROJECT_SOURCE_DIR}/utils" CACHE STRING "RhobanUtils directory")

set(GENERATE_PY "${PROJECT_SOURCE_DIR}/../../python/generate.py")
set(COMMAND_STORE "${PROJECT_SOURCE_DIR}/../../common/commands.xml")

# Sources
set(MAIN_SOURCES 
    main/Command.cpp
    main/main.cpp
    )

set(RHOBAN_SOURCES
    rhoban/config/Configurations.cpp
    rhoban/robot/Robot.cpp
    rhoban/motors/Motors.cpp
    rhoban/motors/Motor.cpp
    )

set(TESTS_SOURCES
    common/communication/tests/allTests.cpp
    )

set(COMMANDS_SOURCES
    )

include_directories("${PROJECT_SOURCE_DIR}")

# Adding common & utils
foreach(DIRECTORY common utils)
    add_subdirectory("${PROJECT_SOURCE_DIR}/${DIRECTORY}")
    include_directories("${PROJECT_SOURCE_DIR}/${DIRECTORY}")
endforeach(DIRECTORY)

set(ALL_SOURCES ${RHOBAN_SOURCES})
set(EXE_SOURCES ${SOURCES} ${ALL_SOURCES} ${MAIN_SOURCES})

if(TESTS)
    set(EXE_SOURCES ${EXE_SOURCES} ${TESTS_SOURCES} ${COMMANDS_SOURCES})
endif(TESTS)

set(EXTRA_LIBS RhobanCommon RhobanUtils)
add_executable(sdk ${EXE_SOURCES})
target_link_libraries(sdk ${EXTRA_LIBS})