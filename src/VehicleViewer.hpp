#ifndef LRT_RTEST_VEHICLEVIEWER_HPP
#define LRT_RTEST_VEHICLEVIEWER_HPP

#include <QAbstractListModel>
#include <QDebug>
#include <QWidget>
#include <memory>

#include "RegulationKernelWrapper.hpp"
#include "StdinReader.hpp"

namespace Ui {
class VehicleViewer;
}

namespace lrt {
namespace rtest {
class RoverModel;

class VehicleViewer : public QWidget
{
  Q_OBJECT

  public:
  enum Wheel
  {
    FL,
    FR,
    RL,
    RR
  };

  public:
  explicit VehicleViewer(QWidget* parent = nullptr);
  virtual ~VehicleViewer();

  void readAssignmentLine(QString line);
  void parseAssignmentToken(QString key, QString value);

  void setPassed(bool passed);

  private:
  ::Ui::VehicleViewer* ui;

  int16_t m_steering = 0;
  int16_t m_speed = 0;

  RoverModel* m_roverModel;
  RegulationKernelWrapper m_wrapper;

  int16_t m_benchmarkSteering = -32768;
  int16_t m_benchmarkSpeed = -32768;
  bool m_benchmarkRunning = false;

  void issueNextBenchmarkStep();
  void finishBenchmark();
  void startBenchmark();
  void updateBenchmarkStats();

  StdinReader m_stdinReader;

  private Q_SLOTS:
  // Sliders
  // -------------------------------------------------
  void on_speedSlider_sliderMoved(int speed);
  void on_steeringSlider_sliderMoved(int steering);

  // Regulation Kernel Wrapper
  // -------------------------------------------------
  void on_wrapperButton_released();

  // Assignment Parser
  // -------------------------------------------------
  void on_assignmentParseButton_released();

  // Benchmark
  // -------------------------------------------------
  void on_benchmarkStartButton_released();

  // Speed
  // -------------------------------------------------
  void on_speedFLSpinner_valueChanged(int speed);
  void on_speedFRSpinner_valueChanged(int speed);
  void on_speedRLSpinner_valueChanged(int speed);
  void on_speedRRSpinner_valueChanged(int speed);

  // Steering
  // -------------------------------------------------
  void on_steeringFLSpinner_valueChanged(int steering);
  void on_steeringFRSpinner_valueChanged(int steering);
  void on_steeringRLSpinner_valueChanged(int steering);
  void on_steeringRRSpinner_valueChanged(int steering);
};
class RoverModel : public QAbstractListModel
{
  Q_OBJECT

  public:
  enum RoverModelRoles
  {
    WheelSpeedRole = Qt::UserRole + 1,
    WheelSteeringRole
  };

  RoverModel() {}
  virtual ~RoverModel() override {}

  virtual QHash<int, QByteArray> roleNames() const override
  {
    QHash<int, QByteArray> roles;
    roles[WheelSpeedRole] = "speed";
    roles[WheelSteeringRole] = "steering";
    return roles;
  }

  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override
  {
    Q_UNUSED(parent);
    return 4;
  }

  virtual QVariant data(const QModelIndex& index,
                        int role = Qt::DisplayRole) const override
  {
    if(index.row() < 0 || index.row() > 4)
      return QVariant();

    VehicleViewer::Wheel wheel = static_cast<VehicleViewer::Wheel>(index.row());
    switch(role) {
      case WheelSpeedRole:
        return m_wheelSpeed[wheel];
      case WheelSteeringRole:
        return m_wheelSteering[wheel];
      default:
        return QVariant();
    }
  }
  virtual bool setData(const QModelIndex& index,
                       const QVariant& val,
                       int role = Qt::EditRole) override
  {
    if(index.row() < 0 || index.row() > 4) {
      return false;
    }

    VehicleViewer::Wheel wheel = static_cast<VehicleViewer::Wheel>(index.row());
    switch(role) {
      case WheelSpeedRole:
        m_wheelSpeed[wheel] = val.toInt();
        break;
      case WheelSteeringRole:
        m_wheelSteering[wheel] = val.toInt();
        break;
      default:
        return false;
    }
    dataChanged(index, index, { role });
    return true;
  }

  private:
  int16_t m_wheelSteering[4] = { 128, 128, 128, 128 };
  int16_t m_wheelSpeed[4] = { 0, 0, 0, 0 };
};
}// namespace rtest
}// namespace lrt

#endif
