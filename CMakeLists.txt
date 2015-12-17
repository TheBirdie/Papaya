cmake_minimum_required(VERSION 2.8.11)

project(Papaya)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt53DCore)
find_package(Qt53DQuick)
find_package(Qt53DRenderer)
find_package(Qt53DInput)
find_package(Qt5OpenGL)
find_package(QGLViewer)

FIND_PACKAGE(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

include_directories(${QGLVIEWER_INCLUDE_DIR})

# Tell CMake to create the helloworld executable
add_executable(Papaya main.cpp Scene.h Scene.cpp)
target_link_libraries(Papaya ${QGLVIEWER_LIBRARY_RELEASE} ${OPENGL_LIBRARIES})

# Use the Widgets module from Qt 5.
qt5_use_modules(Papaya Widgets 3DCore 3DQuick 3DRenderer 3DInput OpenGL)