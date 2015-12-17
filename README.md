# Papaya Project
## How to build/run it
Make sure you have QGLViewer library correctly configured (built) in /path/to/qglviewer


Configure the project with
```
cmake -DQGLVIEWER_INCLUDE_DIR=/path/to/qglviewer -DCMAKE_BUILD_TYPE=[Debug|Release] /path/to/sources
```

## Thoubleshooting
- (Windows) ``Le programme s'est terminé subitement.`` or ``QGLViewer2.dll is not found``
You need to copy ``QGLViewer2.dll`` and ``QGLViewer2d.dll`` from /path/to/qglviewer to your system path (C:\Windows\System32) or to the build directory.