#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdarg>
#include <cstdlib>
#include <cstring>

class Error
{
  const char *msg;

  static const char *msg_sprintf(const char *format, std::va_list args);
public:
  Error(const char *format, ...);
  ~Error();

  const char *what();
};

#endif
