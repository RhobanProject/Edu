cmake_minimum_required(VERSION 2.8)
project(RhobanSDK)

# Options
option(SDK
    "Enable SDK compiling" ON)

option(TESTS
    "Enable tests" OFF)

option(ROBOARD
    "Compile for ROBOARD" OFF)

option(WITH_OPENCV
    "Compile with opencv (vision display)" OFF)

if (NOT RHOBANMATH)
    add_definitions(-DNO_RHOBANMATH)
endif (NOT RHOBANMATH)

set(RHOBAN_MATH_DIRECTORY
	"${PROJECT_SOURCE_DIR}/../../../Math"
		CACHE STRING "Rhoban Math Directory")

option(RHOBANMATH
    "Enables RhobanMath support" OFF)

# Useful for getting common compiling
set(RHOBAN_UTILS_DIRECTORY "${PROJECT_SOURCE_DIR}/utils" CACHE STRING "RhobanUtils directory")
set(RHOBAN_COMMON_DIRECTORY "${PROJECT_SOURCE_DIR}/common" CACHE STRING "RhobanCommon directory")
set(YAML_HEADERS "${RHOBAN_UTILS_DIRECTORY}/yaml-cpp/include" CACHE STRING "Yaml headers directory" CACHE STRING "")

set(GENERATE_PY "${PROJECT_SOURCE_DIR}/../python/generate.py")
set(COMMAND_STORE "${PROJECT_SOURCE_DIR}/../common/commands.xml")

# Sources
set(MAIN_SOURCES 
    main/Command.cpp
    main/main.cpp
    )

set(RHOBAN_SOURCES
    rhoban/config/Configurations.cpp
    rhoban/robot/Robot.cpp
    rhoban/robot/Robots.cpp
    rhoban/robot/Moves.cpp
    rhoban/robot/Sensor.cpp
    rhoban/robot/Sensors.cpp
    rhoban/motors/Motors.cpp
    rhoban/motors/Motor.cpp
    rhoban/commands/Command.cpp
    rhoban/commands/CommandsManager.cpp
    rhoban/commands/Commands.cpp
    rhoban/commands/HelpCommand.cpp
    rhoban/commands/RhobanMain.cpp
    rhoban/commands/RobotCommand.cpp
    rhoban/commands/RobotsCommand.cpp
    rhoban/vision/Vision.cpp
    rhoban/robot/test/testyaml.cpp 
    )

set(TESTS_SOURCES
    common/communication/tests/allTests.cpp
    )

set(COMMANDS_SOURCES
    )

if (WITH_OPENCV)
    #Require OpenCV for vision display
    find_package(OpenCV 2)
    add_definitions(-DWITH_OPENCV)
endif (WITH_OPENCV)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/rhoban")
if(RHOBANMATH)
	include_directories("${RHOBAN_MATH_DIRECTORY}")
endif(RHOBANMATH)

# Adding common & utils
add_subdirectory("${RHOBAN_COMMON_DIRECTORY}" common)
include_directories("${RHOBAN_COMMON_DIRECTORY}")
include_directories("${PROJECT_BINARY_DIR}/common")

set (ENABLE_ZMQ ON)
set (ZMQ_VERSION "3.2")
add_subdirectory("${RHOBAN_UTILS_DIRECTORY}" utils)
include_directories("${RHOBAN_UTILS_DIRECTORY}")
include_directories("${PROJECT_BINARY_DIR}/utils")

include_directories("${YAML_HEADERS}")

set(ALL_SOURCES ${RHOBAN_SOURCES})
set(EXE_SOURCES ${ALL_SOURCES} ${MAIN_SOURCES} ${COMMANDS_SOURCES})

if(TESTS)
    set(EXE_SOURCES ${EXE_SOURCES} ${TESTS_SOURCES})
endif(TESTS)

set(EXTRA_LIBS RhobanCommon RhobanUtils)
add_executable(sdk ${EXE_SOURCES})
add_library(RhobanSDK ${ALL_SOURCES})
add_library(RhobanSDKMain ${EXE_SOURCES})
target_link_libraries(sdk ${EXTRA_LIBS})
target_link_libraries(RhobanSDK ${EXTRA_LIBS})
target_link_libraries(RhobanSDKMain ${EXTRA_LIBS})

if (WITH_OPENCV)
    target_link_libraries(sdk ${OpenCV_LIBS})
    target_link_libraries(RhobanSDK ${OpenCV_LIBS})
    target_link_libraries(RhobanSDKMain ${OpenCV_LIBS})
    include_directories("${OpenCV_DIR}/include")
endif (WITH_OPENCV)

