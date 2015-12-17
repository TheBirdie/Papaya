#ifndef SCENE_H
#define SCENE_H

#include <qglviewer.h>


class Scene : public QGLViewer
{
protected :
  virtual void draw();
  virtual void init();
  virtual void animate();
  virtual QString helpString() const;
};

#endif
