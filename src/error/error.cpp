#include "error.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

Error::Error(const char *format, ...)
{
  char buf[1024];

  std::va_list args;
  va_start(args, format);

  vsprintf(buf, format, args);

  va_end(args);

  this->msg = buf;
}

Error::Error(std::string filename, int line, const char *format, ...)
{
  this->filename = filename;
  this->line = line;

  char buf[1024];

  std::va_list args;
  va_start(args, format);

  vsprintf(buf, format, args);

  va_end(args);

  this->msg = buf;
}

Error::~Error() { }

const char *Error::what() { return this->msg.c_str(); }
