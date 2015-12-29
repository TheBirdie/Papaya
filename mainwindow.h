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

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    std::vector<Scene*> scenes;
    QMdiArea* centralArea;
    QDockWidget* dock;
    QMenuBar* menu;

signals:

public slots:
    void actionOpen();

private:
    void createMenuBar();
    void createDocker();
    bool openModel(const QString& fileName);

    // List of images
    QList<QPixmap> imagesList;
};

#endif // MAINWINDOW_H
