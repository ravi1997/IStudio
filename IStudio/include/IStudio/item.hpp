#ifndef DB221D27_414E_4B2B_B0F2_C25ED5630BBA
#define DB221D27_414E_4B2B_B0F2_C25ED5630BBA

#pragma once

#include <Logger/Logger.hpp>
#include<IStudio/grammar.hpp>

class Item {
  using lookAheadType = Follow;
  std::size_t ruleNumber;
  std::size_t position;
  std::size_t size;
  lookAheadType lookAhead;

public:
  Item(std::size_t r, std::size_t s, std::size_t p = 0, lookAheadType l = lookAheadType{Grammar::DOLLAR_INDEX});

  friend std::ostream &operator<<(std::ostream &o, Item i) {
    o << "Rule Number : " << i.ruleNumber << std::endl;
    o << "Position : " << i.position << std::endl;
    o << "LookAheads : ";
    for (auto l : i.lookAhead)
      o << l << " / ";
    o << std::endl;
    return o;
  }

  std::string toString()const;

  auto getRuleIndex() const { return ruleNumber; }

  auto getPosition() const { return position; }

  auto getSize() const { return size; }

  auto getLookAheads() const { return lookAhead; }

  bool isReduced()const{ return position==size;}

  bool operator==(const Item &i) const ;
  bool operator!=(const Item &i) const ;

  Item getNext() const;
  Item getPrevious() const;

  lookAheadType calculateLookAhead(const Grammar &g) const ;
};

template <details::LoggerTypes t>
details::Logger_Helper<t> operator<<(details::Logger_Helper<t> log,State s){
  for(auto item : s)
    log<<item.toString()<<"\n";
  return log;
}
bool operator==(State l, State r);
bool operator!=(State l, State r);

#endif /* DB221D27_414E_4B2B_B0F2_C25ED5630BBA */
