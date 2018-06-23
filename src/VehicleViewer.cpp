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
  , m_wrapper(this)
{
  ui->setupUi(this);

  // Initialize the rover model.
  m_roverModel = new RoverModel();

  qmlRegisterUncreatableType<RoverModel>(
    "RTest", 1, 0, "RoverModel", "Only given from UserManager.");

  // Add the QML View
  QQuickWidget* view = new QQuickWidget(this);

  // Inject the rover model.
  view->rootContext()->setContextProperty("RoverModel",
                                          QVariant::fromValue(m_roverModel));

  // Load the QML.
  view->setResizeMode(QQuickWidget::SizeRootObjectToView);
  view->setSource(QUrl("qrc:/rtest/viewer.qml"));

  ui->gridLayout->addWidget(view, 0, 1);

  // Regulation Kernel Wrapper
  connect(&m_wrapper,
          &RegulationKernelWrapper::motor_fl_changed,
          ui->speedFLSpinner,
          &QSpinBox::setValue);
  connect(&m_wrapper,
          &RegulationKernelWrapper::motor_fr_changed,
          ui->speedFRSpinner,
          &QSpinBox::setValue);
  connect(&m_wrapper,
          &RegulationKernelWrapper::motor_rl_changed,
          ui->speedRLSpinner,
          &QSpinBox::setValue);
  connect(&m_wrapper,
          &RegulationKernelWrapper::motor_rr_changed,
          ui->speedRRSpinner,
          &QSpinBox::setValue);
  connect(&m_wrapper,
          &RegulationKernelWrapper::servo_fl_changed,
          ui->steeringFLSpinner,
          &QSpinBox::setValue);
  connect(&m_wrapper,
          &RegulationKernelWrapper::servo_fr_changed,
          ui->steeringFRSpinner,
          &QSpinBox::setValue);
  connect(&m_wrapper,
          &RegulationKernelWrapper::servo_rl_changed,
          ui->steeringRLSpinner,
          &QSpinBox::setValue);
  connect(&m_wrapper,
          &RegulationKernelWrapper::servo_rr_changed,
          ui->steeringRRSpinner,
          &QSpinBox::setValue);
  connect(&m_wrapper,
          &RegulationKernelWrapper::state_changed,
          [&](QProcess::ProcessState state) {
            switch(state) {
              case QProcess::NotRunning:
                ui->wrapperButton->setCheckable(true);
                ui->wrapperButton->setText(tr("Run Wrapped Regulation Kernel"));
                break;
              case QProcess::Starting:
                ui->wrapperButton->setCheckable(true);
                break;
              case QProcess::Running:
                ui->wrapperButton->setCheckable(true);
                ui->wrapperButton->setText(
                  tr("Stop Wrapped Regulation Kernel"));
                break;
            }
          });
}

VehicleViewer::~VehicleViewer()
{
  disconnect(&m_wrapper, &RegulationKernelWrapper::state_changed, 0, 0);
  delete m_roverModel;
  delete ui;
}

void
VehicleViewer::on_speedSlider_sliderMoved(int speed)
{
  m_speed = speed;
  m_wrapper.speed_changed(speed);
}
void
VehicleViewer::on_steeringSlider_sliderMoved(int steering)
{
  m_steering = steering;
  m_wrapper.steering_changed(steering);
}

void
VehicleViewer::on_wrapperButton_released()
{
  if(m_wrapper.isRunning()) {
    m_wrapper.stop();
  } else {
    m_wrapper.start(ui->wrapperCommand->document()->toPlainText());
  }
}

void
VehicleViewer::on_speedFLSpinner_valueChanged(int speed)
{
  m_roverModel->setData(
    m_roverModel->index(FL, 0), speed, RoverModel::WheelSpeedRole);
}
void
VehicleViewer::on_speedFRSpinner_valueChanged(int speed)
{
  m_roverModel->setData(
    m_roverModel->index(FR, 0), speed, RoverModel::WheelSpeedRole);
}
void
VehicleViewer::on_speedRLSpinner_valueChanged(int speed)
{
  m_roverModel->setData(
    m_roverModel->index(RL, 0), speed, RoverModel::WheelSpeedRole);
}
void
VehicleViewer::on_speedRRSpinner_valueChanged(int speed)
{
  m_roverModel->setData(
    m_roverModel->index(RR, 0), speed, RoverModel::WheelSpeedRole);
}
void
VehicleViewer::on_steeringFLSpinner_valueChanged(int steering)
{
  m_roverModel->setData(
    m_roverModel->index(FL, 0), steering, RoverModel::WheelSteeringRole);
}
void
VehicleViewer::on_steeringFRSpinner_valueChanged(int steering)
{
  m_roverModel->setData(
    m_roverModel->index(FR, 0), steering, RoverModel::WheelSteeringRole);
}
void
VehicleViewer::on_steeringRLSpinner_valueChanged(int steering)
{
  m_roverModel->setData(
    m_roverModel->index(RL, 0), steering, RoverModel::WheelSteeringRole);
}
void
VehicleViewer::on_steeringRRSpinner_valueChanged(int steering)
{
  m_roverModel->setData(
    m_roverModel->index(RR, 0), steering, RoverModel::WheelSteeringRole);
}
}
}
