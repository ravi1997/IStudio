#ifndef E6011B3A_9926_459E_8C58_8F82C5655AAD
#define E6011B3A_9926_459E_8C58_8F82C5655AAD

#include <IStudio/types.hpp>
#include <Logger/Logger.hpp>
class Rule {
private:
  std::string rule;
  std::size_t left;
  std::vector<std::size_t> right;

public:
  Rule(std::string_view s, std::size_t l1, std::vector<size_t> &&r1)
      : rule{s}, left{l1}, right{move(r1)} {}
  
  First getFirst(const Grammar &g) const;
  auto getLeft() const { return left; }
  auto getRight() const { return right; }
  auto getRule() const { return rule; }
  


  template <details::LoggerTypes t>
  friend details::Logger_Helper<t> &operator<<(details::Logger_Helper<t> &o,
                                               Rule r) {
    o << r.getRule();
    return o;
  }

  friend std::ostream &operator<<(std::ostream &o,
                                               Rule r) {
    o << r.getRule();
    return o;
  }

  bool operator==(const Rule &r) const { return rule == r.rule; }

  bool operator!=(const Rule &r) const { return rule != r.rule; }

  std::string toItem(std::size_t n)const;

  std::size_t getNextSymbol(Item)const;
};

#endif /* E6011B3A_9926_459E_8C58_8F82C5655AAD */
