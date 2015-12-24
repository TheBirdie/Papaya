#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QDockWidget>
#include <QMenuBar>
#include <QList>
#include <QPixmap>
#include "Scene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    Scene* scene;
    QDockWidget* dock;
    QMenuBar* menu;

signals:

public slots:

private:
    void createMenuBar();
    void createDocker();

    // List of images
    QList<QPixmap> imagesList;
};

#endif // MAINWINDOW_H
