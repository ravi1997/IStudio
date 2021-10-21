#ifndef A9F23FC2_DE7C_46F2_8810_1DE184A44733
#define A9F23FC2_DE7C_46F2_8810_1DE184A44733

// #include "grammar.hpp"
#include <IStudio/exception.hpp>
#include <IStudio/item.hpp>
#include <IStudio/parser.hpp>
#include <IStudio/csv.hpp>

Parser::Parser(Grammar &g, Logger &l)
    : grammar{g}, log{l}, syntaxAnalyser{grammar, log} {}

Parser::Parser(const Parser &p)
    : grammar{p.grammar}, log{p.log}, syntaxAnalyser(grammar, log) {}

AST operator|(Generator<Token> tokens, Parser &p) {
  return std::move(tokens) | p.syntaxAnalyser;
}

Parser::SyntaxAnalyser::SyntaxAnalyser(Grammar &g, Logger &l)
    : grammar{g}, log{l} {
      auto createEntry = [&](std::size_t state,std::string col,std::string value){
        auto symbols = grammar.getSymbols();
        auto SIndex = grammar.getSymbolIndexByName(col);
        if(symbols[SIndex].isNonterminal())
          table[state][SIndex] = EntryType{std::stoi(value),EntryHeaderType::SHIFT};
        else{
          char t = value[0];
          switch(t){
            case 's':
              table[state][SIndex] = EntryType{std::stoi(value.substr(1,value.length()-1)),EntryHeaderType::SHIFT};
              break;
            case 'r':
              table[state][SIndex] = EntryType{std::stoi(value.substr(1,value.length()-1)),EntryHeaderType::REDUCE};
              break;
            case 'e':
              table[state][SIndex] = EntryType{std::stoi(value.substr(1,value.length()-1)),EntryHeaderType::ERROR};
              break;
            case 'a':
              table[state][SIndex] = EntryType{0,EntryHeaderType::ACCEPT};
              break;         
          }
        }
      };
      CSVReader csv{"table.csv"};
      for(auto [row,v1] : csv.getTable()){
        for(auto [col,value]:v1){
          //std::cout<<row<<col<<std::endl;
          createEntry(row, col, value);
        }
      }
    }

void Parser::SyntaxAnalyser::print() const {
  printTable();
}

void Parser::SyntaxAnalyser::printTable() const {
  for (auto [state, value] : table) {
    for (auto [symbol, entry] : value)
      std::cout << "table[I" << state << "][" << symbol << "] = " << entry
                << std::endl;
  }
}

void Parser::print() const { syntaxAnalyser.print(); }

std::ostream &operator<<(std::ostream &o, Parser::SyntaxAnalyser::EntryType e) {
  auto [i, t] = e;
  using et = Parser::SyntaxAnalyser::EntryHeaderType;
  if (t == et::SHIFT)
    o << "S" << i;
  else if (t == et::REDUCE)
    o << "R" << i;
  else if (t == et::ERROR)
    o << "E" << i;
  else
    o << "ACCEPT";
  return o;
}

#endif /* A9F23FC2_DE7C_46F2_8810_1DE184A44733 */
