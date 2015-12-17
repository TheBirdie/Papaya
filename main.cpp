#include "Scene.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  Scene viewer;
  viewer.setWindowTitle("animation");
  viewer.show();

  return application.exec();
}
