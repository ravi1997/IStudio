#ifndef __RULE_H__
#define __RULE_H__

#include<IStudio/rule.hpp>
#include<IStudio/grammar.hpp>
#include <IStudio/item.hpp>

First Rule::getFirst(const Grammar &g) const {
  First result;
  bool epsilon = false;
  // cout<<"this Rule : "<<*this<<endl;
  for (auto r : right) {
    epsilon = false;
    auto symbol = g.getSymbols()[r];
    if (symbol.isTerminal())
      result.push_back(r);
    else {
      for (auto f : g.getFirst(symbol.getName()))
        result.push_back(f);
    }
    if (auto itr = find(result.begin(), result.end(), Grammar::EPSILON_INDEX);
        itr != result.end()){
          epsilon = true;
          result.erase(itr);
        }
    else
      break;
  }
  if(epsilon)
    result.push_back(Grammar::EPSILON_INDEX);
  return result;
}

std::string Rule::toItem(std::size_t n)const{
  const auto ruleTokens = split(rule, "->");
  std::string result = ruleTokens[0] + " -> ";
  std::size_t i = 0;
  for (auto r : split(trim(ruleTokens[1]), " ")) {
    if(i==n)
      result+=" . ";
    result += r + " ";
    i++;
  }
  if (i == n)
    result += " . ";
  return result;
}

std::size_t Rule::getNextSymbol(Item it) const{
  if(it.getPosition()==it.getSize()){
    throw GrammarError{"Item has no Forward move left"};
  }
  else
    return right[it.getPosition()];
}

#endif // __RULE_H__