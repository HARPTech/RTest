#include "RegulationKernelWrapper.hpp"
#include <QDebug>

namespace lrt {
namespace rtest {
RegulationKernelWrapper::RegulationKernelWrapper(QObject* parent)
  : QObject(parent)
  , m_registry(std::make_shared<rregistry::Registry>())
  , m_consoleAdapter(
      std::make_shared<rsupport::ConsoleAdapter>(m_registry, true))
{
  m_consoleAdapter->addCallback([&](std::string line) {
    if(m_process) {
      m_process->write(line.c_str(), line.length());
    }
  });
  m_consoleAdapter->setMode(rsupport::ConsoleAdapter::CALLBACK);

  m_registry->registerAdapter(m_consoleAdapter);
  m_registry->addReceiver(this);
}
RegulationKernelWrapper::~RegulationKernelWrapper()
{
  m_registry->removeReceiver(this);
  if(m_process) {
    m_process->kill();
    m_process->waitForFinished();
  }
}

void
RegulationKernelWrapper::start(QString command)
{
  if(isRunning()) {
    stop();
  }

  qDebug() << "Starting: " << command;

  auto process = std::make_unique<QProcess>(this);

  // Setup environment for console embedded mode of RSupport.
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("RSUPPORT_CONSOLE_ONLY", "ON");
  process->setProcessEnvironment(env);
  process->setReadChannel(QProcess::StandardOutput);

  // Setup callbacks.
  connect(process.get(), &QIODevice::readyRead, [&]() {
    while(m_process->canReadLine()) {
      // As soon as a line can be read, the line has to be parsed in the
      // ConsoleAdapter.
      QString line = m_process->readLine();
      m_consoleAdapter->parseLine(line.toStdString());
    }
  });

  connect(process.get(),
          &QProcess::stateChanged,
          [&](QProcess::ProcessState state) { state_changed(state); });

  process->start(command);
  m_process = std::move(process);
}

void
RegulationKernelWrapper::stop()
{
  if(m_process) {
    m_process->kill();
    m_process.reset();
  }
}

bool
RegulationKernelWrapper::isRunning()
{
  return m_process ? m_process->state() == QProcess::Running : false;
}

void
RegulationKernelWrapper::onUpdate(rregistry::Type type, uint32_t property)
{
  using namespace rregistry;
  switch(type) {
    case Type::Int16:
      // Motors
      switch(property) {
        case static_cast<uint32_t>(Int16::MVMT_MOTOR_PWM_FL):
          motor_fl_changed(m_registry->get(Int16::MVMT_MOTOR_PWM_FL));
          break;
        case static_cast<uint32_t>(Int16::MVMT_MOTOR_PWM_FR):
          motor_fr_changed(m_registry->get(Int16::MVMT_MOTOR_PWM_FR));
          break;
        case static_cast<uint32_t>(Int16::MVMT_MOTOR_PWM_RL):
          motor_rl_changed(m_registry->get(Int16::MVMT_MOTOR_PWM_RL));
          break;
        case static_cast<uint32_t>(Int16::MVMT_MOTOR_PWM_RR):
          motor_rr_changed(m_registry->get(Int16::MVMT_MOTOR_PWM_RR));
          break;
        default:
          break;
      }
      break;
    case Type::Uint8:
      // Servos.
      switch(property) {
        case static_cast<uint32_t>(Uint8::MVMT_SERVO_FL_POSITION):
          servo_fl_changed(m_registry->get(Uint8::MVMT_SERVO_FL_POSITION));
          break;
        case static_cast<uint32_t>(Uint8::MVMT_SERVO_FR_POSITION):
          servo_fr_changed(m_registry->get(Uint8::MVMT_SERVO_FR_POSITION));
          break;
        case static_cast<uint32_t>(Uint8::MVMT_SERVO_RL_POSITION):
          servo_rl_changed(m_registry->get(Uint8::MVMT_SERVO_RL_POSITION));
          break;
        case static_cast<uint32_t>(Uint8::MVMT_SERVO_RR_POSITION):
          servo_rr_changed(m_registry->get(Uint8::MVMT_SERVO_RR_POSITION));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

void
RegulationKernelWrapper::steering_changed(int steering)
{
  m_registry->set(rregistry::Int16::MVMT_STEER_DIRECTION, steering);
}
void
RegulationKernelWrapper::speed_changed(int speed)
{
  m_registry->set(rregistry::Int16::MVMT_FORWARD_VELOCITY, speed);
}
}
}
