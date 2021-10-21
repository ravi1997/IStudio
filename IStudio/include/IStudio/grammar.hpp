#ifndef E8563EC5_5FA0_4745_8EC1_3713576224EE
#define E8563EC5_5FA0_4745_8EC1_3713576224EE

#include <IStudio/rule.hpp>
#include <IStudio/symbol.hpp>
#include <IStudio/types.hpp>
class Grammar {
private:
  Logger log;
  std::vector<Rule> rules;
  std::vector<Symbol> symbols;

  void sort();
public:
  constinit static std::size_t START_INDEX, EPSILON_INDEX, DOLLAR_INDEX;
  Grammar(const Grammar &);
  Grammar(const Grammar&&);
  Grammar(std::string_view s, Logger l);
  
  auto getSymbols() const { return symbols; }
  auto getRules() const { return rules; }

  Symbol& getSymbolByName(std::string n);
  Symbol& getSymbolByIndex(std::size_t);
  std::size_t getSymbolIndexByName(std::string n) const ;

  First getFirst(std::string s) const ;
  Follow getFollow(std::string s) const;

  First getFirst(const Symbol &) const;
  Follow getFollow(const Symbol &) const;

  std::vector<size_t> FindAllRules(std::string s) const;

  std::string getState(State s)const;
};

#endif /* E8563EC5_5FA0_4745_8EC1_3713576224EE */
