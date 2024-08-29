#include "error.hpp"

Error::Error(const char *format, ...)
{
  std::va_list args;
  va_start(args, format);

  vsprintf(this->msg, format, args);

  va_end(args);
}

Error::~Error()
{}

const char *Error::what() { return this->msg; }
