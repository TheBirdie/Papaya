#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMdiArea>
#include <QDockWidget>
#include <QMenuBar>
#include <QList>
#include <QPixmap>

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

private:
    void createMenuBar();
    void createDock();
    bool openModel(const QString& fileName);

    void loadImageList(const QList<QPixmap>& imageList);

    // List of images
    QList<QPixmap> imagesList;
};

#endif // MAINWINDOW_H
