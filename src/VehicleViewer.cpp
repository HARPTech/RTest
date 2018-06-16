#include "VehicleViewer.hpp"
#include "ui_VehicleViewer.h"
#include <QDebug>

namespace lrt {
namespace rtest {
VehicleViewer::VehicleViewer(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::VehicleViewer)
{
  ui->setupUi(this);
}

VehicleViewer::~VehicleViewer()
{
  delete ui;
}

void
VehicleViewer::on_speedSlider_sliderMoved(int speed)
{
  qDebug() << "Speed Changed: " << speed;
}
void
VehicleViewer::on_steeringSlider_sliderMoved(int steering)
{
  qDebug() << "Steering Changed: " << steering;
}
}
}
