#include "StdinReader.hpp"
#include "VehicleViewer.hpp"
#include <QTextStream>
#include <QXmlSimpleReader>
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
  QTextStream stdinStream(stdin, QIODevice::ReadOnly);

  QString xml = "<input>";

  while(!stdinStream.atEnd()) {
    QString line = stdinStream.readLine();
    if(line.length() > 0 && line.at(0) == "~") {
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
    }
  }

  xml.append("</input>");

  // Parse XML commands.
  QXmlInputSource source;
  source.setData(xml);
  QXmlSimpleReader simpleReader;
  simpleReader.setContentHandler(this);
  simpleReader.parse(source);
}

}// namespace rtest
}// namespace lrt
