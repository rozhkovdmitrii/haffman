//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_LOGGER_H
#define HAFFMAN_LOGGER_H
//----------------------------------------------------------------------------------------------------------------------
#include <sstream>
#include <iostream>
//----------------------------------------------------------------------------------------------------------------------
class Log {
public:
  enum class Type {
    DBGERR,
    APPERR,
    APPINF
  };

  Log(Type type) : _type(type) {};
  ~Log()
  {
    switch (_type)
    {
    case Type::DBGERR:
      if (_isDbgErrEnabled) std::cerr << "DBGERR: " << _oss.str() << std::endl;
      break;
    case Type::APPERR:
      if (_isAppErrEnabled) std::cerr << "APPERR: " << _oss.str() << std::endl;
      break;
    case Type::APPINF:
      if (_isAppInfEnabled) std::cout << "APPINF: " << _oss.str() << std::endl;

    }
  }

  static void setEnabled(Type type, bool enabled) {
    switch (type)
    {
    case Type::DBGERR:
      _isDbgErrEnabled = enabled;
      break;
    case Type::APPERR:
      _isAppErrEnabled = enabled;
      break;
    case Type::APPINF:
      _isAppInfEnabled = enabled;
      break;
    }
  }

  template <typename T>
  Log & operator<<(const T & value)
  {
    _oss << value;
    return *this;
  }

  operator bool() const {
    return false;
  }

private:
  static bool _isAppErrEnabled;
  static bool _isDbgErrEnabled;
  static bool _isAppInfEnabled;
  Type _type;
  std::ostringstream _oss;

};
//----------------------------------------------------------------------------------------------------------------------
#define LOG(TYPE) Log(Log::Type::TYPE)
//----------------------------------------------------------------------------------------------------------------------
#endif //HAFFMAN_LOGGER_H
//----------------------------------------------------------------------------------------------------------------------
