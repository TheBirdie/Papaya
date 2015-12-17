#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMenuBar>
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
};

#endif // MAINWINDOW_H
