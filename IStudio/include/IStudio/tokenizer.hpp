#ifndef B4C1B5EE_05C0_4E78_8FAF_0A11C4736B05
#define B4C1B5EE_05C0_4E78_8FAF_0A11C4736B05

#include <Types/generator.hpp>
#include<IStudio/symbol.hpp>

class Tokenizer {
private:
  const std::vector<Symbol> symbols;
  std::vector<Symbol> skipSymbols;
  auto begin() const { return ((std::vector<Symbol> &)symbols).begin(); }

  auto end() const { return ((std::vector<Symbol> &)symbols).end(); }

public:
  Tokenizer(const std::vector<Symbol> &s,std::vector<Symbol> t) : symbols{s} {
    for(auto v:t)
      skipSymbols.push_back(v);
  }
  Tokenizer(const Tokenizer &t) : symbols{t.symbols},skipSymbols{t.skipSymbols}{}
  Tokenizer(Tokenizer &&t)
      : symbols{move(t.symbols)}, skipSymbols{std::move(t.skipSymbols)} {}

  auto getSkipSymbols()const{
    return skipSymbols;
  }

  friend Generator<Token> operator|(std::string input, Tokenizer t) {
    unsigned int c = 1, l = 1;
    //std::cout<<t.getSkipSymbols().size()<<std::endl;
    while (input != "") {
      std::string max = "";
      size_t z = 0;
      size_t index = 0;
      bool skip = false;
      //std::cout<<input<<std::endl;
      for (auto s : t) {
        try {
          auto a = s.match(input);
          //std::cout<<"s : "<<s<<std::endl;
          //std::cout<<"a : "<<a<<std::endl;
          if (max.length() < a.length()) {
            max = a;
            z = index;
          }
        } catch (...) {
        }
        // cout<<"found : "<<a<<" in "<<s<<" : "<<s.getPattern()<<endl;
        index++;
      }
      if(max.length()==0){
        for (auto s : t.getSkipSymbols()) {
          //std::cout<<"Skipping "<<std::endl;
          try {
            auto a = s.match(input);
            if (max.length() < a.length()) {
              max = a;
              z = index;
              skip = true;
            }
          } catch (...) {
          }
          // cout<<"found : "<<a<<" in "<<s<<" : "<<s.getPattern()<<endl;
          index++;
        }
      }
      input = input.substr(max.length(), input.length()-max.length());
      if (max == "" && input[0]=='\n') {
        l++;
        c = 1;
        input = input.substr(1,input.length()-1);
        continue;
      }
      else if (skip){
        if(max == "\t")
          c=c+4;
        else if(max == "\n"){
          l++;
          c=1;
        }
        else
          c++;
        continue;      
      }
      co_yield Token{z, max, details::Location{"Example", c, l}};
      c += max.length();
    }
  }
};

#endif /* B4C1B5EE_05C0_4E78_8FAF_0A11C4736B05 */
