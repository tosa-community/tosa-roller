#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error
{
  std::string msg;

public:
  std::string filename;
  int line = 0;

  Error(const char *format, ...);
  Error(std::string filename, int line, const char *format, ...);
  ~Error();

  const char *what();
};

#endif
