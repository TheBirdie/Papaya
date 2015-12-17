# Papaya Project
## Requirements
- QGLViewer - http://libqglviewer.com/
- glew - http://glew.sourceforge.net/
- GLFW - http://www.glfw.org/


## How to build/run it
Make sure you have QGLViewer library correctly configured (built) in /path/to/qglviewer


Configure the project with
```
cmake -DGLEW_LIBRARY=/path/to/glew/lib -DGLEW_INCLUDE_DIR=/path/to/glew/include -DQGLVIEWER_INCLUDE_DIR=/path/to/qglviewer -DCMAKE_BUILD_TYPE=[Debug|Release] /path/to/sources
```
