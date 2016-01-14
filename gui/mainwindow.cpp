#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFileDialog>

#include "mainwindow.h"
#include "parsing/xmlloader.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_imageViewer = NULL;
    centralArea = new QMdiArea();

    setCentralWidget(centralArea);

    //foreach (Reconstruction::Camera const& c, m_views)
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0706.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0707.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0708.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0709.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0710.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0711.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0712.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0713.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0714.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0715.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0716.jpg"));
    m_imagesList.append(QImage("Mouton/Mouton - AT -export undistorted photos/DSC_0717.jpg"));

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
    QAction* openModel = mFile->addAction("Ouvrir modèle 3D");
    QAction* openReconstr = mFile->addAction("Ouvrir photos de reconstruction");
    QAction* close = mFile->addAction("Quitter");

    QObject::connect(openModel, SIGNAL(triggered()), this, SLOT(actionOpenModel()));
    QObject::connect(openReconstr, SIGNAL(triggered()), this, SLOT(actionOpenReconstruction()));
    QObject::connect(close, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Windows menu
    m_menu = new QMenu("Affichage");
    QAction* a_dock = m_menu->addAction("Dock images");
    a_dock->setCheckable(true);
    a_dock->setChecked(true);

    QObject::connect(a_dock, SIGNAL(triggered(bool)), dock, SLOT(setVisible(bool)));
    QObject::connect(dock, SIGNAL(visibilityChanged(bool)), a_dock, SLOT(setChecked(bool)));

    // Menu bar
    menuBar()->addMenu(mFile);
    menuBar()->addMenu(m_menu);
}

/*
 * Creation of image display docker
 */
void MainWindow::createDock(){

    dock = new ImageDock("Dock images", this);

    QObject::connect(dock, SIGNAL(imageClicked(QImage const&)), this, SLOT(displayImg(QImage const&)));

    addDockWidget(Qt::RightDockWidgetArea,dock);
    dock->addImageList(m_imagesList);
}

MainWindow::~MainWindow(){
    if (m_imageViewer)
        delete m_imageViewer;
}

/*
 * Open a model in a new subwindow
 */
bool MainWindow::openModel(const QString& fileName)
{
    //Create a new scene
    Scene* s = new Scene;
    if (s->LoadModel(fileName))
    {
        scenes.push_back(s);

        // Add the scene in a new subwindow
        centralArea->addSubWindow(s);
        s->show();

        // Create an action in windows menu
        QAction* a = m_menu->addAction(QFileInfo(fileName).fileName());
        a->setCheckable(true);
        a->setChecked(true);
        QObject::connect(a, SIGNAL(triggered(bool)), s, SLOT(toggleMinimize(bool)));
        QObject::connect(s, SIGNAL(destroyed(QObject*)), a, SLOT(deleteLater()));
        return true;
    }
    delete s;
    return false;
}

bool MainWindow::openReconstruction(QString const& filename)
{
    // 1. Parse XML file
    Reconstruction loader(filename);
    loader.read(m_views);
    return true;
}


/*
 * SLOT : open a new model
 */
void MainWindow::actionOpenModel(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open Model", "", "Model files (*.obj)");
    if (!fileName.isEmpty())
        openModel(fileName);
}

void MainWindow::displayImg(QImage const& image){
    if (!m_imageViewer)
        m_imageViewer = new ImageViewer();

    m_imageViewer->loadImage(image);
    m_imageViewer->show();
}

void MainWindow::actionOpenReconstruction(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open reconstruction", "", "Reconstruction files (*.xml)");
    if (!fileName.isEmpty())
        openReconstruction(fileName);
}
