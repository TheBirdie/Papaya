#include "mainwindow.h"
#include "Scene.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    scene = new Scene();
    setCentralWidget(scene);
}

MainWindow::~MainWindow(){
    delete scene;
}

