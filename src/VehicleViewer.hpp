#ifndef LRT_RTEST_VEHICLEVIEWER_HPP
#define LRT_RTEST_VEHICLEVIEWER_HPP

#include <QWidget>

namespace Ui {
class VehicleViewer;
}

namespace lrt {
namespace rtest {
class VehicleViewer : public QWidget
{
  Q_OBJECT

  public:
  explicit VehicleViewer(QWidget* parent = 0);
  ~VehicleViewer();

  private:
  Ui::VehicleViewer* ui;
};
}
}

#endif
