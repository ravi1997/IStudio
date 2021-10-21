#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <IStudio/grammar.hpp>
#include <IStudio/symbol.hpp>
#include <IStudio/types.hpp>

First Symbol::getFirst(Grammar g) const {
  First result;
  if (sType == SymbolType::TERMINAL)
    result.push_back(g.getSymbolIndexByName(name));
  else if (sType == SymbolType::NONTERMINAL)
    for (auto e : g.getFirst(name))
      result.push_back(e);
  return result;
}

Follow Symbol::getFollow(Grammar g) const { return g.getFollow(name); }

std::string Symbol::match([[maybe_unused]] std::string input) const {
  if (sType == SymbolType::NONTERMINAL)
    throw GrammarError{std::string{"Symbol \'"} + name + "\' not a terminal"};

  switch (tType) {
  case TerminalType::OPERATOR:
  case TerminalType::NORMAL: {
    // cout<<"ok : "<<pattern<<endl;
    std::smatch sm;
    std::regex_match(input, sm, std::regex{pattern});
    // cout<<"ok2 : "<<sm[1]<<endl;
    return sm[1];
  }
  case TerminalType::EPSILON:
    return std::string{""};
  case TerminalType::DOLLAR:
    return std::string{""};

  default:
    throw GrammarError{"invalid option"};
  }
}

#endif // __SYMBOL_H__