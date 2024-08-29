#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cstdio>

class Error
{
  char msg[1024];
public:
  Error(const char *format, ...);
  ~Error();

  const char *what();
};

#endif
