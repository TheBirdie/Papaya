#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMdiArea>
#include <QDockWidget>
#include <QMenuBar>
#include <QList>
#include <QImage>

#include <vector>

#include "Scene.h"
#include "imagedock.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    std::vector<Scene*> scenes;
    QMdiArea* centralArea;
    ImageDock* dock;
    QMenuBar* menu;

signals:

public slots:
    void actionOpen();
    void displayImg(QImage* img);

private:
    void createMenuBar();
    void createDock();
    bool openModel(const QString& fileName);

    void loadImageList(const QList<QImage>& imageList);

    // List of images
    QList<QImage> imagesList;
};

#endif // MAINWINDOW_H
