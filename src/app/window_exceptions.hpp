#include <exception>
#include <string>

namespace Window {

class InitException : public std::exception {
  std::string m_msg;

 public:
  InitException(const std::string& msg) : m_msg(msg) {}

  virtual const char* what() const throw() { return m_msg.c_str(); }
};

}