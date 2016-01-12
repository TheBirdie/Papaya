#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <QApplication>
#include <iostream>

#include "gui/Scene.h"
#include "gui/mainwindow.h"

int main(int argc, char** argv)
{
    QApplication application(argc,argv);

    MainWindow mainWindow;
    mainWindow.setWindowTitle("Papaya");
    mainWindow.show();

    return application.exec();
}
