#include <QDebug>
#include <QApplication>

#include "VehicleViewer.hpp"

using namespace lrt::rtest;

int
main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  VehicleViewer viewer;

  viewer.showNormal();

  return app.exec();
}
