#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <IStudio/types.hpp>
#include <IStudio/grammar.hpp>
#include <IStudio/item.hpp>
#include <Logger/Logger.hpp>

constinit std::size_t Grammar::START_INDEX = 0;
constinit std::size_t Grammar::EPSILON_INDEX = 0;
constinit std::size_t Grammar::DOLLAR_INDEX = 0;

Grammar::Grammar(const Grammar &g) : log{g.log} {
  for (auto r : g.rules)
    rules.push_back(r);
  for (auto s : g.symbols)
    symbols.push_back(s);
}

Grammar::Grammar(const Grammar &&g)
    : log{std::move(g.log)}, rules{std::move(g.rules)}, symbols{std::move(
                                                            g.symbols)} {}

Grammar::Grammar(std::string_view s, Logger l) : log{l} {
  log.If(level >= 1, Info) << "Setting up Grammar" << lend;
  symbols.push_back(Symbol{"EPSILON", Symbol::SymbolType::TERMINAL,
                           Symbol::TerminalType::EPSILON});
  symbols.push_back(Symbol{"DOLLAR", Symbol::SymbolType::TERMINAL,
                           Symbol::TerminalType::DOLLAR});
  size_t l1;
  for (auto r : split(s, "\n")) {
    auto trimmedLine = trim(r);
    const auto ruleTokens = split(trimmedLine, "->");
    if (ruleTokens.size() == 2) {
      auto left = Symbol{trim(ruleTokens[0]), Symbol::SymbolType::NONTERMINAL};

      if (auto it = find(symbols.begin(), symbols.end(), left);
          it == symbols.end()) {
        symbols.push_back(left);
      } else {
        l1 = distance(symbols.begin(), it);
        symbols[l1].setType(Symbol::SymbolType::NONTERMINAL);
      }

      for (auto right : split(trim(ruleTokens[1]), " ")) {
        if (auto sym = Symbol{right};
            right != "" &&
            find(symbols.begin(), symbols.end(), sym) == symbols.end()) {
          symbols.push_back(sym);
        }
      }
    } else if (ruleTokens.size() == 1) {
      auto left = Symbol{trim(ruleTokens[0]), Symbol::SymbolType::NONTERMINAL};

      if (auto it = find(symbols.begin(), symbols.end(), left);
          it == symbols.end()) {
        symbols.push_back(left);
      } else {
        l1 = distance(symbols.begin(), it);
        symbols[l1].setType(Symbol::SymbolType::NONTERMINAL);
      }
    } else {
      throw GrammarError{std::string{"invalid production rule: "} +
                         trimmedLine};
    }
  }

  sort();

  for (auto r : split(s, "\n")) {
    auto trimmedLine = trim(r);
    if (trimmedLine == "")
      continue;
    // std::cout<<trimmedLine<<std::endl;
    const auto ruleTokens = split(trimmedLine, "->");
    std::vector<size_t> r1;
    r1.clear();
    for (auto right : split(trim(ruleTokens[1]), " ")) {
      r1.push_back(getSymbolIndexByName(right));
    }
    rules.push_back(
        Rule{trimmedLine, getSymbolIndexByName(trim(ruleTokens[0])), move(r1)});
  }
  START_INDEX = 0;
  EPSILON_INDEX = getSymbolIndexByName("EPSILON");
  DOLLAR_INDEX = getSymbolIndexByName("DOLLAR");
}

std::string Grammar::getState(State s) const {
  std::string result;
  for (auto item : s) {
    auto rule = rules[item.getRuleIndex()];
    result += rule.toItem(item.getPosition()) + " { ";
    for (auto lk : item.getLookAheads())
      result += symbols[lk].getName() + " / ";
    result += "}\n";
  }
  return result;
}

void Grammar::sort() {
  std::vector<Symbol> nonterminal, terminal;
  for (auto s : symbols) {
    if (s.isNonterminal())
      nonterminal.push_back(s);
    else
      terminal.push_back(s);
  }
  Symbol id = getSymbolByName("id");
  terminal.erase(std::find(terminal.begin(), terminal.end(), id));
  symbols.clear();
  for (auto s : nonterminal)
    symbols.push_back(s);
  for (auto s : terminal)
    symbols.push_back(s);
  symbols.push_back(id);
}

Symbol &Grammar::getSymbolByName(std::string n) {
  for (Symbol &s : symbols)
    if (s == n)
      return s;
  throw GrammarError{std::string{"Symbol not found : "} + n};
}

Symbol &Grammar::getSymbolByIndex(std::size_t i) {
  if (i < symbols.size())
    return symbols[i];
  else
    throw GrammarError{std::string{"No Symbol at position : "} +
                       std::to_string(i)};
}

std::size_t Grammar::getSymbolIndexByName(std::string n) const {
  std::size_t i = 0;
  for (auto s : symbols)
    if (s == n)
      return i;
    else
      i++;
  throw GrammarError{std::string{"Symbol not found : "} + n};
}

First Grammar::getFirst(std::string s) const {
  First result;
  auto i = getSymbolIndexByName(s);
  if (symbols[i].isNonterminal()) {
    for (auto r : rules) {
      if (r.getLeft() == i)
        for (auto e : r.getFirst(*this))
          result.push_back(e);
    }
  } else
    result.push_back(i);
  return result;
}

Follow Grammar::getFollow(std::string s) const {
  Follow result;
  if (Symbol{s} == symbols[Grammar::START_INDEX]) {
    result.push_back(Grammar::DOLLAR_INDEX);
  } else {
    auto i = getSymbolIndexByName(s);
    for (auto r : rules) {
      size_t pos = 0;

      while (pos < r.getRight().size()) {

        while (pos < r.getRight().size()) {
          auto rs = r.getRight()[pos];
          if (rs == i)
            break;
          pos++;
        }
        if (auto size = r.getRight().size(); pos < size - 1) {
          std::vector<size_t> sym;

          pos++;

          for (auto jk = pos; jk < size; jk++)
            sym.push_back(r.getRight()[jk]);

          for (auto fs : Rule{"", r.getLeft(), move(sym)}.getFirst(*this))
            result.push_back(fs);
          if (auto itr =
                  find(result.begin(), result.end(), Grammar::EPSILON_INDEX);
              itr != result.end()) {
            result.erase(itr);
            for (auto fs : getFollow(symbols[r.getLeft()].getName()))
              if (std::find(result.begin(), result.end(), fs) == result.end())
                result.push_back(fs);
          }
        } else if (pos == size) {
        } else if (r.getLeft() != i) {
          for (auto r1 : getFollow(symbols[r.getLeft()].getName()))
            if (std::find(result.begin(), result.end(), r1) == result.end())
              result.push_back(r1);
          pos++;
        } else {
          pos++;
        }
      }
    }
  }
  return result;
}

std::vector<size_t> Grammar::FindAllRules(std::string s) const {
  std::vector<size_t> result;
  auto left = getSymbolIndexByName(s);
  size_t rn = 0;
  for (auto r : rules) {
    if (r.getLeft() == left)
      result.push_back(rn);
    rn++;
  }
  return result;
}

First Grammar::getFirst(const Symbol &s) const { return getFirst(s.getName()); }

Follow Grammar::getFollow(const Symbol &s) const {
  return getFollow(s.getName());
}

#endif // __GRAMMAR_H__