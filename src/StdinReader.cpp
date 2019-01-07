#include "StdinReader.hpp"
#include "VehicleViewer.hpp"
#include <QTextStream>
#include <QXmlSimpleReader>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace lrt {
namespace rtest {

StdinReader::StdinReader(VehicleViewer& vehicleViewer)
  : m_vehicleViewer(vehicleViewer)
  , m_stdinNotifier(STDIN_FILENO, QSocketNotifier::Read)
{
  connect(&m_stdinNotifier,
          &QSocketNotifier::activated,
          this,
          &StdinReader::activated);
}

StdinReader::~StdinReader() {}

bool
StdinReader::startElement(const QString& ns,
                          const QString& localName,
                          const QString& qname,
                          const QXmlAttributes& attrs)
{
  if(localName == "set") {
    // Sets a variable.
    QString key = attrs.value("key");
    QString val = attrs.value("val");
    m_vehicleViewer.parseAssignmentToken(key, val);
  } else if(localName == "status") {
    // Status should be set into output window and CSS must be changed.
    m_parsingStatus = true;
  }
  return true;
}

bool
StdinReader::endElement(const QString& ns,
                        const QString& localName,
                        const QString& qname)
{
  if(localName == "status") {
    m_parsingStatus = false;
  }
  return true;
}

bool
StdinReader::characters(const QString& characters)
{
  if(m_parsingStatus) {
    m_vehicleViewer.parseAssignmentToken("+rverify_status+", characters);
  }
  return true;
}

void
StdinReader::activated(int)
{
  QString xml = "<input>";

  bool foundXML = false;
  bool running = true;

  int count = 0;

  while(running) {
    // Check if reading can continue afterwards. This is a solution for Linux
    // systems and not yet portable, because it relies heavily on ioctl.
    ioctl(STDIN_FILENO, FIONREAD, &count);
    if(count <= 0) {
      running = false;
      continue;
    }

    char buffer[count];
    ssize_t l = read(STDIN_FILENO, &buffer, count);
    if(l <= 0) {
      qDebug() << "Error during reading from stdin.";
    }

    QString content = QString::fromUtf8(buffer);
    QStringList lines = content.split("\n");

    for(auto line : lines) {
      if(line.length() > 0 && line.at(0) == QChar('~')) {
        line.remove(0, 1);
        // The line is written in quick-set notation.

        // Split strings by "," symbol.
        if(line.contains(",")) {
          QStringList lines = line.split(",");
          for(auto line : lines) {
            m_vehicleViewer.readAssignmentLine(line);
          }
        }
        m_vehicleViewer.readAssignmentLine(line);
      } else {
        // Line is in expanded (XML) syntax.
        if(line != "") {
          xml.append(line);
        }
        xml.append("\n");
        foundXML = true;
      }
    }
  }

  xml.append("</input>");

  if(!foundXML)
    return;

  // Parse XML commands.
  QXmlInputSource source;
  source.setData(xml);
  QXmlSimpleReader simpleReader;
  simpleReader.setContentHandler(this);
  simpleReader.parse(source);
}

}// namespace rtest
}// namespace lrt
