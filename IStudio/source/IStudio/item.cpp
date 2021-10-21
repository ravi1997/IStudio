#ifndef __ITEM_H__
#define __ITEM_H__

#include<IStudio/item.hpp>

bool operator==(State l, State r) {
  if (l.size() != r.size())
    return false;

  for (auto i : l) {
    if (find(r.begin(), r.end(), i) == r.end())
      return false;
  }
  return true;
}

bool operator!=(State l, State r) { return !(l == r); }

Item::Item(std::size_t r, std::size_t s, std::size_t p,
     lookAheadType l)
    : ruleNumber{r}, position{p}, size{s} {

  for (auto l1 : l)
    lookAhead.push_back(l1);
}

std::string Item::toString() const {
  auto result = std::string{"Rule Number : "} + std::to_string(ruleNumber) +
                "\n\tPosition : " + std::to_string(position) +
                "\n\tLookAheads : ";
  for (auto l : lookAhead)
    result += std::to_string(l) + " / ";
  return result;
}

bool Item::operator==(const Item &i) const {
  if (ruleNumber!=i.ruleNumber)
    return false;

  if (position != i.position)
    return false;

  if (lookAhead.size() != i.lookAhead.size())
    return false;

  for (auto lk : lookAhead)
    if (find(i.getLookAheads().begin(),
    i.getLookAheads().end(),
    lk
    ) == i.getLookAheads().end())
      return false;

  return true;
}

bool Item::operator!=(const Item &i) const { return !(*this == i); }

Item Item::getNext() const {
  if (position != size)
    return Item{ruleNumber, size, position + 1, lookAhead};
  else
    throw GrammarError{"Item has no Forward move left"};
}

Item Item::getPrevious() const {
  if (position != 0)
    return Item{ruleNumber, size, position - 1, lookAhead};
  else
    throw GrammarError{"Item has no Backward move left"};
}

Item::lookAheadType Item::calculateLookAhead(const Grammar &g) const {
  lookAheadType result;
  auto rules = g.getRules();
  auto symbols = g.getSymbols();
  auto myRule = rules[ruleNumber];
  bool epsilon = false;

  std::size_t pos = 0;
  for (auto r : myRule.getRight()) {
    if (pos++ <= position)
      continue;

    epsilon = false;

    if (symbols[r].isTerminal()) {
      if (r == Grammar::EPSILON_INDEX) {
        epsilon = true;
      } else {
        epsilon = false;
        result.push_back(r);
        break;
      }
    } else {
      for (auto nfirst : symbols[r].getFirst(g)) {
        if (nfirst == Grammar::EPSILON_INDEX)
          epsilon = true;
        else
          result.push_back(nfirst);
      }
      if (!epsilon)
        break;
    }
  }

  if (epsilon) {
    for (auto lk : lookAhead)
      result.push_back(lk);
  }

  return result;
}

#endif // __ITEM_H__