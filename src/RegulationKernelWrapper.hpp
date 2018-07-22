#ifndef LRT_RTEST_REGULATIONKERNELWRAPPER_HPP
#define LRT_RTEST_REGULATIONKERNELWRAPPER_HPP

#include <QObject>
#include <QProcess>
#include <memory>

#include <RRegistry/RegistryReceiver.hpp>
#include <RSupport/ConsoleAdapter.hpp>

namespace lrt {
namespace rtest {
class RegulationKernelWrapper
  : public QObject
  , public rregistry::RegistryReceiver
{
  Q_OBJECT
  public:
  RegulationKernelWrapper(QObject* parent = nullptr);
  virtual ~RegulationKernelWrapper();

  void start(QString command);
  void stop();

  bool isRunning();

  virtual void onUpdate(rregistry::Type type, uint32_t property);

  void setAcceptProperty(bool accept)
  {
    m_consoleAdapter->setAcceptProperty(accept);
  }

  private:
  std::shared_ptr<rregistry::Registry> m_registry;
  std::shared_ptr<rsupport::ConsoleAdapter> m_consoleAdapter;

  std::unique_ptr<QProcess> m_process;

  Q_SIGNALS:
  void servo_fl_changed(int);
  void servo_fr_changed(int);
  void servo_rl_changed(int);
  void servo_rr_changed(int);

  void motor_fl_changed(int);
  void motor_fr_changed(int);
  void motor_rl_changed(int);
  void motor_rr_changed(int);

  void burst_received();

  void state_changed(QProcess::ProcessState state);

  public Q_SLOTS:
  void steering_changed(int steering);
  void speed_changed(int speed);
};
}
}

#endif
