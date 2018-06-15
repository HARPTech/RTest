#include "VehicleViewer.hpp"
#include "ui_VehicleViewer.h"

namespace lrt {
namespace rtest {
VehicleViewer::VehicleViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VehicleViewer)
{
    ui->setupUi(this);
}

VehicleViewer::~VehicleViewer()
{
    delete ui;
}
}
}
