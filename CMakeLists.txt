cmake_minimum_required(VERSION 2.8.11)

project(Papaya)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt53DCore)
find_package(Qt53DQuick)
find_package(Qt53DRenderer)
find_package(Qt53DInput)

# Tell CMake to create the helloworld executable
add_executable(Papaya WIN32 main.cpp GLView.h GLView.cpp)

# Use the Widgets module from Qt 5.
qt5_use_modules(Papaya Widgets 3DCore 3DQuick 3DRenderer 3DInput)