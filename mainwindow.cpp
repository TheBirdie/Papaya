#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    scene = new Scene();
    scene->LoadModel("model.obj");
    setCentralWidget(scene);

    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));
    imagesList.append(QPixmap("imgTest.png"));

    createMenuBar();
    createDocker();
}

/*
 * Creation of Papaya menu bar
 */
void MainWindow::createMenuBar(){
    // File menu
    QMenu* mFile = new QMenu("Fichier");
    QAction* close = mFile->addAction("Quitter");

    QObject::connect(close, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Menu bar
    menu = new QMenuBar(this);
    menu->addMenu(mFile);

}

/*
 * Creation of image displayer docker
 */
void MainWindow::createDocker(){
    dock = new QDockWidget("Image List", this);

    QScrollArea* scroll = new QScrollArea(dock);
    scroll->setWidgetResizable(true);
    QWidget* container = new QWidget(scroll);
    QVBoxLayout* layout = new QVBoxLayout(container);
    dock->setWidget(scroll);

    for(QList<QPixmap>::iterator img = imagesList.begin(); img != imagesList.end(); img++){
        QLabel* w = new QLabel(container);
        w->setPixmap(*img);
        w->setMinimumHeight(img->height());
        w->setMargin(5);
        layout->addWidget(w);
        container->setMinimumHeight(container->minimumHeight() + img->height());
    }

    container->setLayout(layout);
    scroll->setWidget(container);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea,dock);
}

MainWindow::~MainWindow(){
    delete scene;
}

