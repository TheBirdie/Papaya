#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFileDialog>
#include <QDebug>
#include <QMap>

#include "mainwindow.h"
#include "parsing/xmlloader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_imageViewer(NULL), centralArea(new QMdiArea()),
    m_progressBar(new QProgressBar(this)),
    m_reconstructionOpeningStatus(STATUS_IDLE),
    m_projectionCheckStatus(STATUS_IDLE)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(centralArea);
    layout->addWidget(m_progressBar);
    m_progressBar->hide();
    QWidget *window = new QWidget();
    window->setLayout(layout);

    setCentralWidget(window);

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
        QObject::connect(s, SIGNAL(pointSelected(float,float,float)), this, SLOT(actionPointSelected(float, float, float)));
        return true;
    }
    delete s;
    return false;
}

bool MainWindow::openReconstruction(QString const& filename)
{
    if (m_reconstructionOpeningStatus != STATUS_IDLE)
    {
        qDebug() << "Can't open an XML file. Already loading one. Aborting.";
        m_reconstructionOpeningStatus = STATUS_PENDING_STOP;
        return false;
    }

    // 1. Parse XML file
    m_reconstructionOpeningStatus = STATUS_IN_PROGRESS;
    Reconstruction loader(filename);
    loader.read(m_views);
    dock->deleteImages();
    m_progressBar->show();
    m_progressBar->setMaximum(m_views.size());
    m_progressBar->setValue(0);
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    // 2. Load images
    for (QList<Reconstruction::Camera>::iterator it = m_views.begin(); it != m_views.end(); ++it)
    {
        it->image = QImage(it->imagePath);
        dock->addImage(it->image);
        m_progressBar->setValue(m_progressBar->value() + 1);
        // The loading can be long, let's update the UI to display at least the progress bar
        qApp->processEvents();
        if (m_reconstructionOpeningStatus == STATUS_PENDING_STOP)
        {
            m_progressBar->hide();
            m_reconstructionOpeningStatus = STATUS_IDLE;
            return false;
        }
    }
    m_progressBar->hide();
    m_reconstructionOpeningStatus = STATUS_IDLE;
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

class ImageDist
{
    public:
        ImageDist() {}
        ImageDist(QImage const& i, float d): image(&i), distance(d) {}
        bool operator<(ImageDist const& other) const { return distance < other.distance; }
        QImage const* image;
        float distance;
};

void MainWindow::actionPointSelected(float x, float y, float z)
{
    if (m_projectionCheckStatus != STATUS_IDLE)
    {
        qDebug() << "actionPointSelected: Already processing. Aborting.";
        m_projectionCheckStatus = STATUS_PENDING_STOP;
        return;
    }
    m_projectionCheckStatus = STATUS_IN_PROGRESS;

    qDebug() << "Finding photos that can see (" << x << ", " << y << ", " << z << ")";
    Scene* scene = (Scene*)QObject::sender();
    m_progressBar->show();
    m_progressBar->setMaximum(m_views.size());
    m_progressBar->setValue(0);
    dock->deleteImages();
    qApp->processEvents();
    Vec clicked(x, y, z);

    QVector<ImageDist> images;
    foreach (Reconstruction::Camera const& c, m_views)
    {
        m_progressBar->setValue(m_progressBar->value() + 1);
        qApp->processEvents();
        Vec from(c.center[0], c.center[1], c.center[2]);
        Vec to(clicked);
        bool hit = scene->GetFirstIntersection(from, to, 0.99);
        if (hit)
        {
            qDebug() << "Image not in LOS: hit (" << to.x << "," << to.y << "," << to.z << ")";
            continue;
        }
        float dist = (from-to).mag();
        qDebug() << "Point in LOS: distance" << dist;
        images.push_back(ImageDist(c.image, dist));
        dock->addImage(c.image);

        if (m_projectionCheckStatus == STATUS_PENDING_STOP)
        {
            m_progressBar->hide();
            m_projectionCheckStatus = STATUS_IDLE;
            return;
        }
    }
    qSort(images);
    dock->deleteImages();
    foreach (ImageDist const &id, images)
        dock->addImage(*(id.image));
    m_progressBar->hide();
    m_projectionCheckStatus = STATUS_IDLE;
}
