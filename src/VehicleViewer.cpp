#include "VehicleViewer.hpp"
#include "ui_VehicleViewer.h"
#include <QDebug>

#include <QQmlContext>

#include <QtQuickWidgets/QQuickWidget>

namespace lrt {
namespace rtest {
VehicleViewer::VehicleViewer(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::VehicleViewer)
{
  ui->setupUi(this);

  // Initialize the rover model.
  m_roverModel = new RoverModel(this);

  qmlRegisterUncreatableType<RoverModel>(
    "RTest", 1, 0, "RoverModel", "Only given from UserManager.");

  // Add the QML View
  QQuickWidget* view = new QQuickWidget(this);

  // Inject the rover model.
  view->rootContext()->setContextProperty("RoverModel", QVariant::fromValue(m_roverModel));

  // Load the QML.
  view->setResizeMode(QQuickWidget::SizeRootObjectToView);
  view->setSource(QUrl("qrc:/rtest/viewer.qml"));

  ui->gridLayout->addWidget(view, 0, 1);
}

VehicleViewer::~VehicleViewer()
{
  delete m_roverModel;
  delete ui;
}

void
VehicleViewer::on_speedSlider_sliderMoved(int speed)
{
  m_speed = speed;
}
void
VehicleViewer::on_steeringSlider_sliderMoved(int steering)
{
  m_steering = steering;
}

void
VehicleViewer::on_speedFLSpinner_valueChanged(int speed)
{
  m_wheelSpeed[FL] = speed;
  m_roverModel->dataChanged(m_roverModel->index(FL, 0),
                            m_roverModel->index(FL, 0),
                            { RoverModel::WheelSpeedRole });
}
void
VehicleViewer::on_speedFRSpinner_valueChanged(int speed)
{
  m_wheelSpeed[FR] = speed;
  m_roverModel->dataChanged(m_roverModel->index(FR, 0),
                            m_roverModel->index(FR, 0),
                            { RoverModel::WheelSpeedRole });
}
void
VehicleViewer::on_speedRLSpinner_valueChanged(int speed)
{
  m_wheelSpeed[RL] = speed;
  m_roverModel->dataChanged(m_roverModel->index(RL, 0),
                            m_roverModel->index(RL, 0),
                            { RoverModel::WheelSpeedRole });
}
void
VehicleViewer::on_speedRRSpinner_valueChanged(int speed)
{
  m_wheelSpeed[RR] = speed;
  m_roverModel->dataChanged(m_roverModel->index(RR, 0),
                            m_roverModel->index(RR, 0),
                            { RoverModel::WheelSpeedRole });
}
void
VehicleViewer::on_steeringFLSpinner_valueChanged(int steering)
{
  m_wheelSteering[FL] = steering;
  m_roverModel->dataChanged(m_roverModel->index(FL, 0),
                            m_roverModel->index(FL, 0),
                            { RoverModel::WheelSteeringRole });
}
void
VehicleViewer::on_steeringFRSpinner_valueChanged(int steering)
{
  m_wheelSteering[FR] = steering;
  m_roverModel->dataChanged(m_roverModel->index(FR, 0),
                            m_roverModel->index(FR, 0),
                            { RoverModel::WheelSteeringRole });
}
void
VehicleViewer::on_steeringRLSpinner_valueChanged(int steering)
{
  m_wheelSteering[RL] = steering;
  m_roverModel->dataChanged(m_roverModel->index(RL, 0),
                            m_roverModel->index(RL, 0),
                            { RoverModel::WheelSteeringRole });
}
void
VehicleViewer::on_steeringRRSpinner_valueChanged(int steering)
{
  m_wheelSteering[RR] = steering;
  m_roverModel->dataChanged(m_roverModel->index(RR, 0),
                            m_roverModel->index(RR, 0),
                            { RoverModel::WheelSteeringRole });
}
}
}
