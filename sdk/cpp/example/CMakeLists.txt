# Fichier d'exemple pour compiler un programme en utilisant
# le SDK de Rhoban

cmake_minimum_required(VERSION 2.8)
project(RhobanExample)

# Chemin menant au dossier cpp/ du SDK Rhoban
set(SDK_PATH ..)

# Ajout de la dépendance vers le SDK
add_subdirectory(${SDK_PATH} sdk)

# Sources, ajoutez ici les fichiers constituant votre code
set(SOURCES
    example.cpp
    )

# Utilisation du SDK, Common & Utils pour les chemins des
# includes
include_directories(
    ${RhobanSDK_SOURCE_DIR}
    ${RhobanUtils_SOURCE_DIR}
    ${RhobanCommon_SOURCE_DIR}
    )

add_executable(Program ${SOURCES})
target_link_libraries(Program RhobanSDKMain)
