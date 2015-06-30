#______Configuration

cmake_minimum_required(VERSION 2.6)

PROJECT(SimuViti)
FIND_PACKAGE(Qt4 REQUIRED)
SET(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

ADD_DEFINITIONS(-g -W -Wall)



FILE(
  GLOB_RECURSE
  source_files
  src/*
)

#______OpenGL+Qt4
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)



#______Qt 4
SET(QT_FILE_HEADERS src/MainWindow.h src/SimulationViewer.h src/Controler.h src/FieldInformationWidget.h src/RobotInformationWidget.h src/MapControlWidget.h)
SET(QT_FILE_RESSOURCES )
SET(QT_FILE_FORMS src/MainWindow.ui src/Controler.ui src/FieldInformationWidget.ui src/RobotInformationWidget.ui src/MapControlWidget.ui)

QT4_WRAP_CPP(HEADERS_MOC ${QT_FILE_HEADERS})
QT4_ADD_RESOURCES(RESSOURCES_RCC ${QT_FILE_RESSOURCES})
QT4_WRAP_UI(FORMS_HEADERS ${QT_FILE_FORMS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})



#______Create Exec.
INCLUDE_DIRECTORIES(${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} )

ADD_EXECUTABLE(simu ${source_files} ${HEADERS_MOC} ${RESSOURCES_RCC} ${FORMS_HEADERS})

TARGET_LINK_LIBRARIES(simu ${QT_LIBRARIES} ${OPENGL_LIBRARIES} )
