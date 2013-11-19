
#include <string>

struct Library {
  bool isnoreturn(const std::string &) const { return false; }
  bool isnotnoreturn(const std::string &) const { return false; };
  bool reportErrors(const std::string &) const { return true; }
};
