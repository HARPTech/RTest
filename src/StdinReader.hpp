#ifndef LRT_RTEST_STDINREADER_HPP
#define LRT_RTEST_STDINREADER_HPP

#include <QObject>
#include <QSocketNotifier>
#include <QXmlDefaultHandler>

namespace lrt {
namespace rtest {
class VehicleViewer;

class StdinReader
  : public QObject
  , public QXmlDefaultHandler
{
  Q_OBJECT
  public:
  StdinReader(VehicleViewer& vehicleViewer);
  virtual ~StdinReader();

  private:
  VehicleViewer& m_vehicleViewer;
  QSocketNotifier m_stdinNotifier;

  virtual bool startElement(const QString& ns,
                            const QString& localName,
                            const QString& qname,
                            const QXmlAttributes& attrs);
  virtual bool endElement(const QString& ns,
                          const QString& localName,
                          const QString& qname);
  virtual bool characters(const QString& characters);

  bool m_parsingStatus = false;

  private Q_SLOTS:
  void activated(int);
};
}// namespace rtest
}// namespace lrt

#endif
