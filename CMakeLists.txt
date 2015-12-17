cmake_minimum_required(VERSION 2.8.11)

project(Papaya)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt53DCore)
find_package(Qt53DQuick)
find_package(Qt53DRenderer)
find_package(Qt53DInput)
find_package(Qt5OpenGL)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(QGLViewer)

FIND_PACKAGE(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

include_directories(${QGLVIEWER_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR})

# Tell CMake to create the helloworld executable
add_executable(Papaya main.cpp Scene.h Scene.cpp)
set(LIBRARIES ${OPENGL_LIBRARIES})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIBRARIES ${LIBRARIES} ${QGLVIEWER_LIBRARY_DEBUG})
else()
    set(LIBRARIES ${LIBRARIES} ${QGLVIEWER_LIBRARY_RELEASE})
endif()
target_link_libraries(Papaya ${LIBRARIES})

# Use the Widgets module from Qt 5.
qt5_use_modules(Papaya Widgets 3DCore 3DQuick 3DRenderer 3DInput OpenGL)

# Copy QGLViewer2.dll on Windows
if (WIN32)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(QGLVIEWER_DLL_PATH "${QGLVIEWER_INCLUDE_DIR}/QGLViewerd2.dll")
    else()
        set(QGLVIEWER_DLL_PATH "${QGLVIEWER_INCLUDE_DIR}/QGLViewer2.dll")
    endif()
    message(STATUS "[Windows] Will copy QGLViewer DLLs to project run path (${QGLVIEWER_DLL_PATH})")
    add_custom_command(TARGET Papaya PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${QGLVIEWER_DLL_PATH}" "${CMAKE_BINARY_DIR}"
        COMMENT "Copying ${QGLVIEWER_DLL_PATH} to ${CMAKE_BINARY_DIR}")
endif()
