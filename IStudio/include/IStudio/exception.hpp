#ifndef FD58DBF9_52FB_4B5D_AFF9_A0E518C0F02E
#define FD58DBF9_52FB_4B5D_AFF9_A0E518C0F02E

#include <IStudio/types.hpp>

class Exception {
private:
  unsigned int line = 0;
  std::string fileName = "";
  std::string description = "";

public:
  Exception(unsigned int l, std::string f, std::string des)
      : line{l}, fileName{f}, description{des} {}

  template <typename stream>
  friend stream &operator<<(stream &s, const Exception &e) {
    s << e.fileName << ":" << e.line << ":" << e.description;
    return s;
  }
};

class GrammarError : public Exception {
public:
  GrammarError(std::string s,
               const std::source_location &location =
                   std::source_location::current())
      : Exception(location.line(), location.file_name(),
                  std::string{"GrammarError : "} + s) {}
};



#endif /* FD58DBF9_52FB_4B5D_AFF9_A0E518C0F02E */
