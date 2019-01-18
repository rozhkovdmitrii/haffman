//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_LOGGER_H
#define HAFFMAN_LOGGER_H
//----------------------------------------------------------------------------------------------------------------------
#include <sstream>
#include <iostream>
//----------------------------------------------------------------------------------------------------------------------
class ErrLog {
public:
  enum class Type {
    DBGERR,
    APPERR
  };

  ErrLog(Type type) : _type(type) {};
  ~ErrLog()
  {
    switch (_type)
    {
    case Type::DBGERR:
      if (_isDbgErrEnabled) std::cerr << "DBGERR: " << _oss.str() << std::endl;
      break;
    case Type::APPERR:
      if (_isAppErrEnabled) std::cerr << "APPERR: " << _oss.str() << std::endl;
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
    }
  }

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
  static bool _isAppErrEnabled;
  static bool _isDbgErrEnabled;
  Type _type;
  std::ostringstream _oss;
};
//----------------------------------------------------------------------------------------------------------------------
#define LOG(TYPE) ErrLog(ErrLog::Type::TYPE)
//----------------------------------------------------------------------------------------------------------------------
#endif //HAFFMAN_LOGGER_H
//----------------------------------------------------------------------------------------------------------------------
