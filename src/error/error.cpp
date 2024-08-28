#include "error.hpp"

const char *Error::msg_sprintf(const char *format, std::va_list args)
{
  char *out = nullptr;
  unsigned int length = 0;

  do {
    if (*format == '%') {
      format++;

      if (*format == 'c') {
        out = (char *)std::realloc(out, ++length * sizeof(char));
        out[length - 1] = (char)va_arg(args, int);
      } else if (*format == 's') {
        char *str = va_arg(args, char *);

        for (unsigned int i = 0; i < std::strlen(str); i++) {
          out = (char *)std::realloc(out, ++length * sizeof(char));
          out[length - 1] = str[i];
        }
      }
    } else {
      out = (char *)std::realloc(out, ++length * sizeof(char));
      out[length - 1] = *format;
    }

    format++;
  } while (*format != '\0');

  return out;
}

Error::Error(const char *format, ...)
{
  std::va_list args;
  va_start(args, format);

  this->msg = this->msg_sprintf(format, args);

  va_end(args);
}

Error::~Error() { delete this->msg; }

const char *Error::what() { return this->msg; }
