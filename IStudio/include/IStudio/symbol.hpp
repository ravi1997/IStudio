#ifndef D1B5EEB6_F165_4A5B_A310_88301ED3AEC6
#define D1B5EEB6_F165_4A5B_A310_88301ED3AEC6

#include <IStudio/exception.hpp>
#include <IStudio/types.hpp>

class Symbol {
public:
  enum class SymbolType { TERMINAL, NONTERMINAL };

  enum class TerminalType { NORMAL, DOLLAR, EPSILON, OPERATOR };

  enum class OperatorType { UNARYPRE, UNARYPOST, BINARY, TERNARY };

private:
  std::string name;
  SymbolType sType = SymbolType::TERMINAL;

  TerminalType tType = TerminalType::NORMAL;
  std::string pattern;

  OperatorType oType;

public:
  Symbol(std::string n, SymbolType s = SymbolType::TERMINAL,
         TerminalType t = TerminalType::NORMAL)
      : name{n}, sType{s}, tType{t} {}
  

  Symbol(Symbol&&s): name{std::move(s.name)}, sType{std::move(s.sType)}, tType{std::move(s.tType)}, pattern{std::move(s.pattern)},
        oType{std::move(s.oType)}{}
  Symbol(const Symbol &s)
      : name{s.name}, sType{s.sType}, tType{s.tType}, pattern{s.pattern},
        oType{s.oType} {}
  Symbol &operator=(const Symbol &s) {
    name = s.name;
    pattern = s.pattern;
    sType = s.sType;
    tType = s.tType;
    return *this;
  }

  bool operator==(std::string s)const { return name==s;}
  bool operator==(const Symbol &s) const { return (name == s.name); }
  bool operator!=(const Symbol &s) const { return (name != s.name); }

  std::string getType() const {
    if (isNonterminal())
      return "NONTERMINAL";
    else
      return "TERMINAL";
  }

  void setType(SymbolType s) { sType = s; }

  template <typename stream> friend stream &operator<<(stream &o, Symbol s) {
    o << s.name;
    return o;
  }

  std::string match([[maybe_unused]] std::string input) const;

  void setPattern(std::string s) {
    pattern = "(";
    pattern += s + ")((.*[\n]*)*)";
  }

  std::string getPattern() const { return pattern; }

  First getFirst(Grammar g) const;
  Follow getFollow(Grammar g) const;

  auto getName() const { return name; }

  void setAsOperator(){
    tType = TerminalType::OPERATOR;
  }

  bool isTerminal() const { return sType == SymbolType::TERMINAL; }
  bool isNonterminal() const { return sType == SymbolType::NONTERMINAL; }
};

#endif /* D1B5EEB6_F165_4A5B_A310_88301ED3AEC6 */
