#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <QApplication>
#include <iostream>

#include "Scene.h"
#include "mainwindow.h"

int main(int argc, char** argv)
{


    std::cout << "Init OK" << std::endl;
    QApplication application(argc,argv);
    GLEW_OK;

    MainWindow mainWindow;
    mainWindow.setWindowTitle("Papaya");
    mainWindow.show();
    // Initialise GLFW
    if (!glfwInit())
    {
        std::cerr << "Unable to init GLFW. Aborting" << std::endl;
        return 2;
    }
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to init GLEW. Aborting" << std::endl;
        return 1;
    }

    //Scene viewer;
    //viewer.setWindowTitle("animation");
    //viewer.show();

    return application.exec();
}
