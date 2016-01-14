#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    imageViewer = NULL;
    centralArea = new QMdiArea();

    setCentralWidget(centralArea);

    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0706.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0707.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0708.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0709.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0710.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0711.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0712.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0713.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0714.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0715.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0716.jpg"));
    imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0717.jpg"));

    createDock();
    //Needs to be after create dock
    createMenuBar();
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

    // Windows menu
    mWindows = new QMenu("Affichage");
    QAction* a_dock = mWindows->addAction("Dock images");
    a_dock->setCheckable(true);
    a_dock->setChecked(true);

    QObject::connect(a_dock, SIGNAL(triggered(bool)), dock, SLOT(setVisible(bool)));
    QObject::connect(dock, SIGNAL(visibilityChanged(bool)), a_dock, SLOT(setChecked(bool)));

    // Menu bar
    menuBar()->addMenu(mFile);
    menuBar()->addMenu(mWindows);
}

/*
 * Creation of image display docker
 */
void MainWindow::createDock(){

    dock = new ImageDock("Dock images", this);

    QObject::connect(dock, SIGNAL(imageClicked(QImage*)), this, SLOT(displayImg(QImage*)));

    addDockWidget(Qt::RightDockWidgetArea,dock);
    dock->addImageList(imagesList);
}

MainWindow::~MainWindow(){
    if(imageViewer != NULL)
        delete imageViewer;
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
        QMdiSubWindow* sw = centralArea->addSubWindow(s);
        s->show();

        // Create an action in windows menu
        QAction* a = mWindows->addAction(QFileInfo(fileName).fileName());
        a->setCheckable(true);
        a->setChecked(true);
        QObject::connect(a, SIGNAL(triggered(bool)), s, SLOT(toggleMinimize(bool)));
        QObject::connect(s, SIGNAL(destroyed(QObject*)), a, SLOT(deleteLater()));
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

void MainWindow::displayImg(QImage* image){
    if(imageViewer == NULL)
        imageViewer = new ImageViewer();

    imageViewer->loadFile("Mouton/Mouton - AT -export undistorted photos/DSC_0706.jpg");
    imageViewer->show();
}
