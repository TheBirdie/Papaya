#ifndef SCENE_H
#define SCENE_H

#include <qglviewer.h>
#include "parsing/objloader.h"

struct ObjModel;

class Scene : public QGLViewer
{
public:
  bool LoadModel(QString const& filename);
protected:
  virtual void draw();
  void drawSpiral();
  virtual void init();
  virtual void animate();
  virtual QString helpString() const;
public slots:
  void select(const QMouseEvent *event);
protected:
  Mesh m_model;
};

#endif
