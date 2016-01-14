#ifndef SCENE_H
#define SCENE_H

#include <qglviewer.h>
#include "parsing/objloader.h"

struct ObjModel;

class Scene : public QGLViewer
{
    Q_OBJECT
public:
  Scene(QWidget* parent=NULL);
  bool LoadModel(QString const& filename);
protected:
  virtual void draw();
  void drawSpiral();
  virtual void init();
  virtual void animate();
  virtual QString helpString() const;
signals:
  void pointSelected(float x, float y, float z);
public slots:
  void select(const QMouseEvent *event);
  void toggleMinimize(bool state);
protected:
  Mesh m_model;
};

#endif
