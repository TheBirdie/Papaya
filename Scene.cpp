#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <QDebug>
#include <QtQuick/QtQuick>

#include "Scene.h"
#include "parsing/objloader.h"

bool Scene::LoadModel(QString const& filename)
{
    qDebug() << "Loading model " << filename;
    m_model.LoadObjModel(filename.toStdString().c_str());
    qDebug() << m_model.faceIndex.size() / 3 << "triangles";
    return true;
}

void Scene::draw()
{
    // If we have a model loaded ...
    if (true)
    {
        m_model.Draw();
    }
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
    glMatrixMode(GL_MODELVIEW);
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.1f, 0.0f, 0.5f );
    glClearDepth( 10.0f );
    //glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    GLfloat amb_light[] = { 0.3, 0.3, 0.3, 4.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    //glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Scene::animate()
{

}

QString Scene::helpString() const
{
    return QString("blah");
}
