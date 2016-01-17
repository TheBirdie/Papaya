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

    QObject::connect(dock, SIGNAL(imageClicked(QImage const&, QString const&)), this, SLOT(displayImg(QImage const&, QString const&)));

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
        it->image = QImage(it->imagePath).scaled(150, 100);
        dock->addImage(it->image, it->imagePath);
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

void MainWindow::displayImg(QImage const& image, QString const& filename){
    if (!m_imageViewer)
        m_imageViewer = new ImageViewer();

    m_imageViewer->loadFile(filename);
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
        ImageDist(Reconstruction::Camera const& cam, float d): cam(&cam), distance(d) {}
        bool operator<(ImageDist const& other) const { return distance < other.distance; }
        Reconstruction::Camera const* cam;
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

    /// 1. Find clicked point, init interface
    qDebug() << "Finding photos that can see (" << x << ", " << y << ", " << z << ")";
    Scene* scene = (Scene*)QObject::sender();
    m_progressBar->show();
    m_progressBar->setMaximum(m_views.size());
    m_progressBar->setValue(0);
    dock->deleteImages();
    qApp->processEvents();
    Vec clicked(x, y, z);

    /// 2. Consider each view loaded from XML
    QVector<ImageDist> images;
    foreach (Reconstruction::Camera const& c, m_views)
    {
        /// 2.1 For each image, check LoS
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

        /// 2.2 Check that the point is inside the image bounds
        Vec proj(0,0,0);
        proj.x = (to-from).x*c.rotation[0][0]+(to-from).y*c.rotation[0][1]+(to-from).z*c.rotation[0][2];
        proj.y = (to-from).x*c.rotation[1][0]+(to-from).y*c.rotation[1][1]+(to-from).z*c.rotation[1][2];
        proj.z = (to-from).x*c.rotation[2][0]+(to-from).y*c.rotation[2][1]+(to-from).z*c.rotation[2][2];
        int point[2];
        point[0] = c.focalLength * proj.x/proj.z + c.principalPoint[0];
        point[1] = c.focalLength * proj.y/proj.z + c.principalPoint[1];
        if(point[0]<0 || point[0]>=c.width || point[1]<0 || point[1]>=c.height){
            qDebug() << "Point not in image: point (" << point[0] << "," << point[1] << ")";
            continue;
        }

        /// 2.3 Insert views that pass the tests on a vector
        float dist = (from-to).mag();
        qDebug() << "Point in LOS: distance" << dist;
        images.push_back(ImageDist(c, dist));
        dock->addImage(c.image, c.imagePath);

        if (m_projectionCheckStatus == STATUS_PENDING_STOP)
        {
            m_progressBar->hide();
            m_projectionCheckStatus = STATUS_IDLE;
            return;
        }
    }
    /// 3. Display photos ordered by their distance to current 3D view
    qSort(images);
    dock->deleteImages();
    foreach (ImageDist const &id, images)
        dock->addImage(id.cam->image, id.cam->imagePath);
    m_progressBar->hide();
    m_projectionCheckStatus = STATUS_IDLE;
}
