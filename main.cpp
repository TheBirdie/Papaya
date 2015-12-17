#include "Scene.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  MainWindow mainWindow;
  mainWindow.setWindowTitle("Papaya");
  mainWindow.show();

  //Scene viewer;
  //viewer.setWindowTitle("animation");
  //viewer.show();

  return application.exec();
}
