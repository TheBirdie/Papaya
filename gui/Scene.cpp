#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <QDebug>
#include <QtQuick/QtQuick>

#include "kdtree.h"
#include "Scene.h"
#include "parsing/objloader.h"

Scene::Scene(QWidget *parent):
    QGLViewer(parent)
{}

bool Scene::LoadModel(QString const& filename)
{
    qDebug() << "Loading model " << filename;
    m_model.LoadObjModel(filename.toStdString().c_str());
    qDebug() << m_model.faceIndex.size() / 3 << "triangles";
    qDebug() << "Model radius:" << m_model.GetRadius();
    setSceneRadius(m_model.GetRadius());
    setSceneCenter(qglviewer::Vec(m_model.GetCenter().x, m_model.GetCenter().y, m_model.GetCenter().z));
    return true;
}

void Scene::draw()
{
    // If we have a model loaded ...
    if (m_model.IsLoaded())
        m_model.Draw();
    else // Fallback to default-hardcoded model
        drawSpiral();

}

void Scene::drawSpiral()
{
    // Draws a spiral
    const float nbSteps = 200.0;
    glBegin(GL_QUAD_STRIP);
    for (float i=0; i<nbSteps; ++i)
      {
        float ratio = i/nbSteps;
        float angle = 21.0*ratio;
        float c = cos(angle);
        float s = sin(angle);
        float r1 = 1.0 - 0.8*ratio;
        float r2 = 0.8 - 0.8*ratio;
        float alt = ratio - 0.5;
        const float nor = .5;
        const float up = sqrt(1.0-nor*nor);
        glColor3f(1.0-ratio, 0.2f , ratio);
        glNormal3f(nor*c, up, nor*s);
        glVertex3f(r1*c, alt, r1*s);
        glVertex3f(r2*c, alt+0.05, r2*s);
      }
    glEnd();
}

void Scene::init()
{
    // Nice texture coordinate interpolation
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    m_model.LoadOpenGLTextures();
}

void Scene::animate()
{

}

QString Scene::helpString() const
{
    return QString("Use SELECT + CLICK to select a 3D point");
}

// Emmit a signal when a point is selected on the scene
void Scene::select(const QMouseEvent *event)
{
    bool intersect = false;

    QPoint clickPoint(event->x(), event->y());
    qglviewer::Vec point = camera()->pointUnderPixel(clickPoint, intersect);
    if (intersect)
    {
        qDebug() << "Scene: SELECT " << clickPoint << " => (" << point.x << "," << point.y << "," << point.z << ")";
        emit pointSelected(point.x, point.y, point.z);
    }
    else
        qDebug() << "Scene: SELECT " << clickPoint << " => NO INTERSECTION";
}

// Toggle state of the widget
void Scene::toggleMinimize(bool state){
    if(!state)
        showMinimized();
    else
        showNormal();
}

// Return first intersection between a ray and the model
bool Scene::GetFirstIntersection(Vec const& from, Vec& to, double tmax) const
{
    double t;
    Vec unused;
    Vec d(to - from);
    return m_model.m_tree.hit(Ray(from, d), t, tmax, unused, to);
}
