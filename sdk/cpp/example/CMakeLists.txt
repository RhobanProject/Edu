cmake_minimum_required(VERSION 2.8)
project(RhobanSkeleton)

# Adding dependency
add_subdirectory(.. sdk)

include_directories(
    ${RhobanSDK_SOURCE_DIR}
    ${RhobanUtils_SOURCE_DIR}
    ${RhobanCommon_SOURCE_DIR}
    )

# Sources
set(SOURCES
    main.cpp
    )

add_executable(Program ${SOURCES})
target_link_libraries(Program RhobanSDKMain)
