#include "VehicleViewer.hpp"
#include "ui_VehicleViewer.h"
#include <QDebug>

#include <QQmlContext>
#include <QtQml>
#include <QtQuickWidgets/QQuickWidget>
#include <thread>

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
  connect(&m_wrapper, &RegulationKernelWrapper::burst_received, [&]() {
    issueNextBenchmarkStep();
  });
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
VehicleViewer::issueNextBenchmarkStep()
{
  if(m_benchmarkRunning) {
    m_wrapper.setAcceptProperty(true);
    updateBenchmarkStats();
    if(m_benchmarkSteering < 32767 - 500) {
      m_benchmarkSteering += 500;
      ui->steeringSlider->setValue(m_benchmarkSteering);
      on_steeringSlider_sliderMoved(m_benchmarkSteering);
    } else {
      m_benchmarkSteering = -32768;
      if(m_benchmarkSpeed < 32767 - 500) {
        m_benchmarkSpeed += 500;
        ui->speedSlider->setValue(m_benchmarkSpeed);
        on_speedSlider_sliderMoved(m_benchmarkSpeed);
      } else {
        finishBenchmark();
      }
    }
  }
}
void
VehicleViewer::updateBenchmarkStats()
{
  // Update time taken and percentage completed.
  int64_t done = (32769 + (int64_t)m_benchmarkSpeed) *
                   (int64_t)std::numeric_limits<uint16_t>::max() +
                 (int64_t)m_benchmarkSteering;
  int64_t todo = (int64_t)std::numeric_limits<uint16_t>::max() *
                 (int64_t)std::numeric_limits<uint16_t>::max();
  float percentage = (float)done / (float)todo;
  ui->benchmarkProgress->setValue(percentage * 100);
  QString statusText = tr("Trying out every combination of inputs with one "
                          "step being 500 units.\n") +
                       tr("Currently, ") + QString::number(done) + tr(" of ") +
                       QString::number(todo);
  ui->benchmarkStatus->setText(statusText);
}
void
VehicleViewer::startBenchmark()
{
  m_benchmarkSteering = -32768;
  m_benchmarkSpeed = -32768;
  ui->steeringSlider->setValue(m_benchmarkSteering);
  on_steeringSlider_sliderMoved(m_benchmarkSteering);
  m_benchmarkRunning = true;
  ui->speedSlider->setValue(m_benchmarkSpeed);
  on_speedSlider_sliderMoved(m_benchmarkSpeed);
}
void
VehicleViewer::finishBenchmark()
{
  m_benchmarkRunning = false;
  updateBenchmarkStats();
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
VehicleViewer::on_assignmentParseButton_released()
{
  // Parse the given assignments line by line.
  QString text = ui->assignmentInput->toPlainText();
  QTextStream stream(&text, QIODevice::ReadOnly);
  while(!stream.atEnd()) {
    QString line = stream.readLine();
    readAssignmentLine(line);
  }
}
void
VehicleViewer::readAssignmentLine(QString line)
{
  line.remove(',');
  if(line.contains('=')) {
    QStringList tokens = line.split('=');
    parseAssignmentToken(tokens.at(0).trimmed(), tokens.at(1).trimmed());
  }
}
void
VehicleViewer::parseAssignmentToken(QString key, QString value)
{
  int32_t val = value.toInt();
  if(key == "_motor_fl_") {
    ui->speedFLSpinner->setValue(val);
  }
  if(key == "_motor_fr_") {
    ui->speedFRSpinner->setValue(val);
  } else if(key == "_motor_rl_") {
    ui->speedRLSpinner->setValue(val);
  } else if(key == "_motor_rr_") {
    ui->speedRRSpinner->setValue(val);
  } else if(key == "_servo_fl_") {
    ui->steeringFLSpinner->setValue(val);
  } else if(key == "_servo_fr_") {
    ui->steeringFRSpinner->setValue(val);
  } else if(key == "_servo_rl_") {
    ui->steeringRLSpinner->setValue(val);
  } else if(key == "_servo_rr_") {
    ui->steeringRRSpinner->setValue(val);
  } else if(key == "_steer_direction_") {
    ui->steeringSlider->setValue(val);
  } else if(key == "_forward_velocity_") {
    ui->speedSlider->setValue(val);
  }
}

void
VehicleViewer::on_benchmarkStartButton_released()
{
  if(m_benchmarkRunning) {
    finishBenchmark();
    ui->benchmarkStartButton->setText(tr("Start Benchmark"));
  } else {
    startBenchmark();
    ui->benchmarkStartButton->setText(tr("Stop Benchmark"));
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
