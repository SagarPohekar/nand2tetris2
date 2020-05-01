#include "XMLWriter.h"

XMLWriter::XMLWriter(std::ostream& output)
  : m_indent{ 0 },
    m_output_file{ output }
{
}

XMLWriter::~XMLWriter()
{
}

void XMLWriter::writeTag(std::string_view tag, std::string_view val, Indent idt, bool opening, bool closing)
{
  switch (idt)
  {
  case XMLWriter::Indent::Decrease:
    m_indent = m_indent - static_cast<int>(idt);
    break;
  case XMLWriter::Indent::Default:
    m_indent = 0;
    break;
  case XMLWriter::Indent::Increase:
    m_indent = m_indent + static_cast<int>(idt);
    break;
  [[falthrough]] case XMLWriter::Indent::NoChange:
  default:
    break;
  }
  
  m_indent = m_indent >= 0 ? m_indent : 0;
  const std::string indent(m_indent, '\t');

  if (opening) {
    m_output_file << indent << "<" << tag << '>';
  }
  if (val.size()) {
    m_output_file << ' ' << val << ' ';
  }
  if (closing) {
    m_output_file << (!opening ? indent : "") << "</" << tag << '>';
  }
  m_output_file << '\n';
}

void XMLWriter::writeOpeningTag(std::string_view tag, Indent idt)
{
  writeTag(tag, "", idt, true, false);
}

void XMLWriter::writeClosingTag(std::string_view tag, Indent idt)
{
  writeTag(tag, "", idt, false, true);
}
