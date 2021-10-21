#ifndef BC0CA2A0_4493_461A_8416_06BC220F41D5
#define BC0CA2A0_4493_461A_8416_06BC220F41D5

#include <IStudio/types.hpp>
class Token {
private:
  std::size_t symbol;
  std::string input;
  details::Location location;

public:
  Token(size_t s, std::string i, details::Location l)
      : symbol{s}, input{i}, location{l} {}

  Token(const Token &t)
      : symbol{t.symbol}, input{t.input}, location{t.location} {}

  Token(Token &&t)
      : symbol{std::move(t.symbol)}, input{std::move(t.input)},
        location{std::move(t.location)} {}

  Token& operator=(Token& t){
    symbol = t.symbol;
    input = t.input;
    location = t.location;
    return *this;
  }

  Token &operator=(Token &&t) {
    symbol = std::move(t.symbol);
    input = std::move(t.input);
    location = std::move(t.location);
    return *this;
  }

  auto getSymbol() const { return symbol; }

  auto getMatchedString() const { return input; }

  auto getLocation() const { return location; }

  template <typename stream> friend stream &operator<<(stream &s, Token t) {
    s << t.location << " : {" << t.symbol << " , " << t.input << "}";
    return s;
  }
};

#endif /* BC0CA2A0_4493_461A_8416_06BC220F41D5 */
