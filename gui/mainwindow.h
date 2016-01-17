#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMdiArea>
#include <QDockWidget>
#include <QMenuBar>
#include <QList>
#include <QImage>
#include <QProgressBar>

#include <vector>

#include "Scene.h"
#include "imagedock.h"
#include "imageviewer.h"
#include "parsing/xmlloader.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);
    virtual ~MainWindow();

    std::vector<Scene*> scenes;
    QMdiArea* centralArea;
    ImageDock* dock;

signals:

public slots:
    void actionOpenModel();
    void actionOpenReconstruction();
    void actionPointSelected(float x, float y, float z);
    void displayImg(QImage const& img, const QString& filename);

private:
    void createMenuBar();
    void createDock();

    bool openModel(const QString& fileName);
    bool openReconstruction(const QString& fileName);
    void loadImageList(const QList<QImage>& imageList);

    // List of camera views - contains associated images
    QList<Reconstruction::Camera>   m_views;
    enum
    {
        STATUS_IDLE,
        STATUS_IN_PROGRESS,
        STATUS_PENDING_STOP,
    };
    int                             m_reconstructionOpeningStatus;
    int                             m_projectionCheckStatus;
    // Viewer
    ImageViewer*    m_imageViewer;
    QMenu*          m_menu;
    QProgressBar*   m_progressBar;
};

#endif // MAINWINDOW_H
