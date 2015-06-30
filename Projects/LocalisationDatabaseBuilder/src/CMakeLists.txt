# Fichier d'exemple pour compiler un programme en utilisant
# le SDK de Rhoban

cmake_minimum_required(VERSION 2.8)
project(RhobanExample)

# Options
option(RHOBANMATH
    "Enables RhobanMath support" ON)

set(RHOBAN_MATH_DIRECTORY
 	"${CMAKE_SOURCE_DIR}/../../../Math/"
    	 CACHE STRING "RhobanMath directory")
    	 
set(RHOBAN_UTILS_DIRECTORY "${PROJECT_SOURCE_DIR}/../../../Utils" CACHE STRING "RhobanUtils directory")

# Chemin menant au dossier cpp/ du SDK Rhoban
set (SDK_PATH ../../../Edu/sdk/cpp CACHE STRING "Sdk path")
set (CODE_BASE ../../../Code)
set (RHOBAN_VISION_DIRECTORY "${CODE_BASE}/Vision" CACHE STRING "RhobanVision directory")
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}:${CMAKE_MODULE_PATH}")
add_definitions (-DVISION_COMPONENT)
set (EXTRA_LIBS ${EXTRA_LIBS} RhobanVision)
add_subdirectory ("${RHOBAN_VISION_DIRECTORY}" vision)
option(WITH_OPENCV "Compile with opencv (vision display)" ON)
find_package(OpenCV 2)
add_definitions(-DWITH_OPENCV)

# Ajout de la dépendance vers le SDK
add_subdirectory(${SDK_PATH} sdk)

# Ajout de la dépendance vers math
add_subdirectory(${RHOBAN_MATH_DIRECTORY} math)

# Utilisation du SDK, Common & Utils pour les chemins des
# includes
include_directories(
	${RHOBAN_VISION_DIRECTORY}
    ${RhobanSDK_SOURCE_DIR}
    ${RhobanUtils_SOURCE_DIR}
    ${RhobanCommon_SOURCE_DIR}
    ${RHOBAN_MATH_DIRECTORY}
    ${SDK_PATH}
    ${SDK_PATH}/rhoban
    ${PROJECT_BINARY_DIR}/sdk/common
	${OpenCV_DIR}/include
)

target_link_libraries(RhobanCommon)


add_executable(video_display video_display.cpp)
add_executable(pngGrabber png_grabber.cpp)
add_executable(goal_interpolation goal_interpolation.cpp)
add_executable(test_pos_estimator test_pos_estimator.cpp)

target_link_libraries(test_pos_estimator RhobanVision RhobanSDKMain RhobanCommon)
target_link_libraries(goal_interpolation RhobanVision RhobanSDKMain RhobanCommon)
target_link_libraries(pngGrabber RhobanSDKMain RhobanMath RhobanCommon RhobanUtils ${OpenCV_LIBS})
target_link_libraries(video_display RhobanSDKMain RhobanMath RhobanCommon RhobanUtils ${OpenCV_LIBS})
add_executable(localizationDatabaseBuilder localizationDatabaseBuilder.cpp)
target_link_libraries(localizationDatabaseBuilder RhobanSDKMain RhobanMath RhobanCommon RhobanUtils ${OpenCV_LIBS} RhobanVision)
