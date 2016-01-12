cmake_minimum_required(VERSION 3.0)
project(Papaya)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

################# FIND EXTERNAL DEPENDENCIES ########################
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt53DCore)
find_package(Qt53DQuick)
find_package(Qt53DRenderer)
find_package(Qt53DInput)
find_package(Qt5OpenGL)
find_package(QGLViewer PATHS ${CMAKE_SOURCE_DIR}/cmake/)
find_package(OpenGL REQUIRED)
find_package(X11)
find_package(GLEW)
if(NOT GLEW_FOUND AND WIN32)
    set(GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/glew/include)
    set(GLEW_LIBRARIES ${CMAKE_SOURCE_DIR}/dep/glew/lib/glew32.lib)
    message(WARNING "Using provided glew binaries - for VC14 (${GLEW_LIBRARIES})")
endif()

# Build GLFW
find_package(GLFW PATHS ${CMAKE_SOURCE_DIR}/cmake/)
if (NOT GLFW_FOUND)
    option(GLFW_USE_INTERNAL "Build internal version of GLFW" ON)
else()
    option(GLFW_USE_INTERNAL "Build internal version of GLFW" OFF)
endif()

set(GLFW_SOURCE_DIR "${CMAKE_SOURCE_DIR}/dep/glfw/")
if (GLFW_USE_INTERNAL)
    ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/dep/glfw)
    set(GLFW_glfw_LIBRARY glfw)
endif()

set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${QGLVIEWER_INCLUDE_DIR})
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${QT_QTXML_INCLUDE_DIR})
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${GLEW_INCLUDE_DIRS})
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${GLFW_INCLUDE_DIR})
include_directories(${INCLUDE_DIRECTORIES})

################# CREATE THE EXECUTABLE #############################
file(GLOB GUI_srcs
    gui/Scene.cpp gui/Scene.h
    gui/imagedock.h gui/imagedock.cpp
    gui/imageclickable.h gui/imageclickable.cpp
    gui/mainwindow.h gui/mainwindow.cpp)
file(GLOB PARSING_srcs parsing/*.h parsing/*.cpp)
file(GLOB GLM_srcs glm/*.h) # Headers only library

add_executable(Papaya
        main.cpp 
        ${GUI_srcs} ${PARSING_srcs} ${GLM_srcs})


################# CONFIGURE LINKED LIBRAIRIES #######################
set(LIBRARIES ${LIBRARIES} ${OPENGL_LIBRARIES})
set(LIBRARIES ${LIBRARIES} ${GLEW_LIBRARIES})
set(LIBRARIES ${LIBRARIES} ${GLFW_glfw_LIBRARY})
if (X11_FOUND)
    set(LIBRARIES ${LIBRARIES} "${X11_X11_LIB}" "${CMAKE_THREAD_LIBS_INIT}" "${X11_Xrandr_LIB}")
    set(LIBRARIES ${LIBRARIES} "${X11_Xinerama_LIB}")

    # Check for XInput (high-resolution cursor motion)
    if (X11_Xinput_FOUND)
        if (X11_Xinput_LIB)
            set(LIBRARIES ${LIBRARIES} "${X11_Xinput_LIB}")
        else()
            # Backwards compatibility (bug in CMake 2.8.7)
            set(LIBRARIES ${LIBRARIES} Xi)
        endif()
    endif()

    # Check for Xf86VidMode (fallback gamma control)
    if (X11_xf86vmode_FOUND)
        if (X11_Xxf86vm_LIB)
            set(LIBRARIES ${LIBRARIES} "${X11_Xxf86vm_LIB}")
        else()
            # Backwards compatibility (see CMake bug 0006976)
            set(LIBRARIES ${LIBRARIES} Xxf86vm)
        endif()
    endif()

    set(LIBRARIES ${LIBRARIES} "${X11_Xcursor_LIB}")
endif()
if (UNIX)
    set(LIBRARIES ${LIBRARIES} "${OPENGL_gl_LIBRARY}")
    set(LIBRARIES ${LIBRARIES}  "${CMAKE_DL_LIBS}")
endif()


if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND QGLVIEWER_LIBRARY_DEBUG)
    set(LIBRARIES ${LIBRARIES} ${QGLVIEWER_LIBRARY_DEBUG})
    set(QGLVIEWER_DLL_PATH "${QGLVIEWER_INCLUDE_DIR}/QGLViewerd2.dll")
else()
    set(LIBRARIES ${LIBRARIES} ${QGLVIEWER_LIBRARY_RELEASE})
    set(QGLVIEWER_DLL_PATH "${QGLVIEWER_INCLUDE_DIR}/QGLViewer2.dll")
endif()

set(LIBRARIES ${LIBRARIES} ${QGLVIEWER_LIBRARY_RELEASE})

target_link_libraries(Papaya ${LIBRARIES})
qt5_use_modules(Papaya Widgets 3DCore 3DQuick 3DRenderer 3DInput OpenGL)

################# PLATFORM SPECIFIC CODE ############################
if (WIN32)
    message(STATUS "[Windows] Will copy QGLViewer DLLs to project run path (${QGLVIEWER_DLL_PATH})")
    add_custom_command(TARGET Papaya PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${QGLVIEWER_DLL_PATH}" "${CMAKE_BINARY_DIR}"
        COMMENT "Copying ${QGLVIEWER_DLL_PATH} to ${CMAKE_BINARY_DIR}")
endif()
