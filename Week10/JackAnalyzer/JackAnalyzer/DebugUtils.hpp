#ifndef __DEBUG_UTILS_HPP__
#define __DEBUG_UTILS_HPP__

namespace Debug
{
  static const bool Debug_{ true };

  namespace
  {
    template<typename T>
    void _Trace(std::ostream& os, const T& data)
    {
      if (Debug::Debug_)
        os << data << '\n';
    }
  }

  template<typename T>
  void Log(const T& data)
  {
    _Trace(std::clog, data);
  }

  template<typename T>
  void Error(const T& data)
  {
    _Trace(std::cerr, data);
  }

  template<typename T>
  void Out(const T& data)
  {
    _Trace(std::cout, data);
  }

  template<typename T>
  void PrintList(const T& data)
  {
    if (Debug::Debug_) {
      for (const auto& elm : data) {
        std::cout << data << ' ';
      }
      std::cout << '\n';
    }
  }
}

#endif // !__DEBUG_UTILS_HPP__