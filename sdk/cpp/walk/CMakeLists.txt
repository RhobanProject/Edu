# Fichier d'exemple pour compiler un programme en utilisant
# le SDK de Rhoban

cmake_minimum_required(VERSION 2.8)
project(RhobanExample)

# Options
option(RHOBANMATH
    "Enables RhobanMath support" OFF)

set(RHOBAN_MATH_DIRECTORY
 	"${CMAKE_SOURCE_DIR}/../../../../Math/"
    	 CACHE STRING "RhobanMath directory")

# Chemin menant au dossier cpp/ du SDK Rhoban
set(SDK_PATH
	 ..
		CACHE STRING "Sdk path")

# Ajout de la dépendance vers le SDK
add_subdirectory(${SDK_PATH} sdk)

# Ajout de la dépendance vers math
add_subdirectory(${RHOBAN_MATH_DIRECTORY} math)

# Sources, ajoutez ici les fichiers constituant votre code
set(SOURCES
        main.cpp
        ClockWalk.cpp
        Function.cpp
        SigmabanWalk.cpp
    )

   
# Utilisation du SDK, Common & Utils pour les chemins des
# includes
include_directories(
    ${RhobanSDK_SOURCE_DIR}
    ${RhobanUtils_SOURCE_DIR}
    ${RhobanCommon_SOURCE_DIR}
    ${RHOBAN_MATH_DIRECTORY}
    ${SDK_PATH}
    ${SDK_PATH}/rhoban
    ${PROJECT_BINARY_DIR}/sdk/common
)

add_executable(Walk ${SOURCES})
target_link_libraries(Walk RhobanSDKMain RhobanMath RhobanCommon RhobanUtils )
