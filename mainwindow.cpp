#include "mainwindow.h"
#include "Scene.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    scene = new Scene();
    scene->LoadModel("model.obj");
    setCentralWidget(scene);
}

MainWindow::~MainWindow(){
    delete scene;
}

