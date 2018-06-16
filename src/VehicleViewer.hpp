#ifndef LRT_RTEST_VEHICLEVIEWER_HPP
#define LRT_RTEST_VEHICLEVIEWER_HPP

#include <QAbstractListModel>
#include <QDebug>
#include <QWidget>
#include <memory>

namespace Ui {
class VehicleViewer;
}

namespace lrt {
namespace rtest {
class RoverModel;

class VehicleViewer : public QWidget
{
  public:
  enum Wheel
  {
    FL,
    FR,
    RL,
    RR
  };

  Q_OBJECT

  public:
  explicit VehicleViewer(QWidget* parent = 0);
  virtual ~VehicleViewer();

  int16_t getWheelSteering(Wheel wheel) const { return m_wheelSteering[wheel]; }
  int16_t getWheelSpeed(Wheel wheel) const { return m_wheelSpeed[wheel]; }

  private:
  Ui::VehicleViewer* ui;

  int16_t m_steering = 0;
  int16_t m_speed = 0;

  int16_t m_wheelSteering[4] = { 128, 128, 128, 128 };
  int16_t m_wheelSpeed[4] = { 0, 0, 0, 0 };

  RoverModel* m_roverModel;

  private Q_SLOTS:
  // Sliders
  // -------------------------------------------------
  void on_speedSlider_sliderMoved(int speed);
  void on_steeringSlider_sliderMoved(int steering);

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

  RoverModel(VehicleViewer* viewer)
    : m_viewer(viewer)
  {
  }
  virtual ~RoverModel() {}

  QHash<int, QByteArray> roleNames() const override
  {
    QHash<int, QByteArray> roles;
    roles[WheelSpeedRole] = "speed";
    roles[WheelSteeringRole] = "steering";
    return roles;
  }

  int rowCount(const QModelIndex& parent = QModelIndex()) const override
  {
    Q_UNUSED(parent);
    return 4;
  }

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override
  {
    if(index.row() < 0 || index.row() > 4)
      return QVariant();

    VehicleViewer::Wheel wheel = static_cast<VehicleViewer::Wheel>(index.row());
    switch(role) {
      case WheelSpeedRole:
        return m_viewer->getWheelSpeed(wheel);
      case WheelSteeringRole:
        return m_viewer->getWheelSteering(wheel);
      default:
        return QVariant();
    }
  }

  private:
  VehicleViewer* m_viewer;
};
}
}

#endif
