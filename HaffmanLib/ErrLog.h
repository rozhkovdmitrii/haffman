

#ifndef HAFFMAN_LOGGER_H
#define HAFFMAN_LOGGER_H

#include <sstream>
#include <iostream>

class ErrLog {
public:
  ErrLog() = default;
  ~ErrLog()
  {
    if (_enabled)
      std::cerr << "ERROR: " << _oss.str() << std::endl;
  }
  static void enable() { _enabled = true; }
  static void disable() { _enabled = false; }

  template <typename T>
  ErrLog & operator<<(const T & value)
  {
    _oss << value;
    return *this;
  }

  operator bool() const {
    return false;
  }

private:
  static bool _enabled;
  std::ostringstream _oss;
};


#endif //HAFFMAN_LOGGER_H
