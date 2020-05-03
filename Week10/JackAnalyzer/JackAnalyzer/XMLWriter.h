#ifndef __XMLWRITER_H__
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>

class XMLWriter
{
public:
  enum class Indent : int { Decrease = -1, Default, Increase = 1, NoChange };
  explicit XMLWriter(std::ostream& output);
  ~XMLWriter();
public:
  void writeTag(std::string_view tag, std::string_view val, Indent idt = Indent::Default, bool opening = true, bool closing = true);
  void writeOpeningTag(std::string_view tag, Indent idt = Indent::Default);
  void writeClosingTag(std::string_view tag, Indent idt = Indent::Default);
  void setIndentation(int n) noexcept { m_indent = n; }
  void setIndentation(Indent n);
private:
  int m_indent;
  std::ostream& m_output_file;
};

#endif // !__XMLWRITER_H__
