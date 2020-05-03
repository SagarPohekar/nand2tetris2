#include "XMLWriter.h"

//-----------------------------------------------------------------------------------
// XMLWriter::Constructor
//-----------------------------------------------------------------------------------
XMLWriter::XMLWriter(std::ostream& output)
  : m_indent{ 0 },
    m_output_file{ output }
{
}

//-----------------------------------------------------------------------------------
// XMLWriter::Destructor
//-----------------------------------------------------------------------------------
XMLWriter::~XMLWriter()
{
}

//-----------------------------------------------------------------------------------
// XMLWriter::writeTag
//-----------------------------------------------------------------------------------
void XMLWriter::writeTag(std::string_view tag, std::string_view val, Indent idt, bool opening, bool closing)
{
  setIndentation(idt);
  
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

//-----------------------------------------------------------------------------------
// XMLWriter::writeOpeningTag
//-----------------------------------------------------------------------------------
void XMLWriter::writeOpeningTag(std::string_view tag, Indent idt)
{
  writeTag(tag, "", idt, true, false);
}

//-----------------------------------------------------------------------------------
// XMLWriter::writeClosingTag
//-----------------------------------------------------------------------------------
void XMLWriter::writeClosingTag(std::string_view tag, Indent idt)
{
  writeTag(tag, "", idt, false, true);
}

//-----------------------------------------------------------------------------------
// XMLWriter::setIndentation
//-----------------------------------------------------------------------------------
void XMLWriter::setIndentation(Indent idt)
{
  switch (idt)
  {
  case XMLWriter::Indent::Decrease:
    m_indent = m_indent - 1;
    break;
  case XMLWriter::Indent::Default:
    m_indent = 0;
    break;
  case XMLWriter::Indent::Increase:
    m_indent = m_indent + 1;
    break;
  [[falthrough]] case XMLWriter::Indent::NoChange:
  default:
    break;
  }
}

