#ifndef NOSUCHELEMENTEXCEPTION_H
#define NOSUCHELEMENTEXCEPTION_H
#include <exception>
#include <stdexcept>
#include <string>

class NoSuchElementException : public std::exception {
public:
  std::string reason;
  NoSuchElementException(std::string s) throw() {
    reason = s;
  }
  virtual ~NoSuchElementException() throw() {}
};

#endif
