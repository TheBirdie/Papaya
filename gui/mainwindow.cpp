#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    centralArea = new QMdiArea();

    setCentralWidget(centralArea);

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
    createDock();

}

/*
 * Creation of Papaya menu bar
 */
void MainWindow::createMenuBar(){
    // File menu
    QMenu* mFile = new QMenu("Fichier");
    QAction* open = mFile->addAction("Ouvrir");
    QAction* close = mFile->addAction("Quitter");

    QObject::connect(open, SIGNAL(triggered()), this, SLOT(actionOpen()));
    QObject::connect(close, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Menu bar
    menu = new QMenuBar(this);
    menu->addMenu(mFile);

}

/*
 * Creation of image display docker
 */
void MainWindow::createDock(){

    dock = new ImageDock("Images List", this);

    QObject::connect(dock, SIGNAL(imageClicked(QPixmap*)), this, SLOT(displayImg(QPixmap*)));

    addDockWidget(Qt::RightDockWidgetArea,dock);
    dock->addImageList(imagesList);
}

// Penser à détruire les scenes, elles n'ont pas de parents
// TODO : les détruires à la fermeture de la subWindow
MainWindow::~MainWindow(){
    for(std::vector<Scene*>::iterator i = scenes.begin(); i != scenes.end(); i++)
        delete *i;
}

/*
 * Open a model in a new subwindow
 */
bool MainWindow::openModel(const QString& fileName){
    //Create a new scene
    Scene* s = new Scene;
    if(s->LoadModel(fileName)){
        scenes.push_back(s);

        // Add the scene in a new subwindow
        centralArea->addSubWindow(s);
        s->show();
    }
    else
        delete s;
}

/*
 * SLOT : open a new model
 */
void MainWindow::actionOpen(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open model", "", "Model files (*.obj)");
    if(!fileName.isEmpty()){
        openModel(fileName);
    }
}

void MainWindow::displayImg(QPixmap* image){
    QLabel* label = new QLabel(this);
    label->setPixmap(*image);
    centralArea->addSubWindow(label);
    label->show();
}
