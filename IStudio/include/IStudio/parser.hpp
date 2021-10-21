#ifndef __PARSER_H__
#define __PARSER_H__

#pragma once

#include <IStudio/CompilerError/error.hpp>
#include <IStudio/Graph/FlowControlGraph.hpp>
#include <IStudio/ast.hpp>
#include <Types/generator.hpp>
#include <IStudio/grammar.hpp>
#include <IStudio/item.hpp>
#include <IStudio/token.hpp>
#include <IStudio/types.hpp>

class Parser {
private:
  std::function<ASTResult(int, Parser &)> func;
  Grammar &grammar;
  Logger &log;

public:
  class SyntaxAnalyser {
  public:
    enum EntryHeaderType { ERROR, ACCEPT, SHIFT, REDUCE };
    using EntryType = std::pair<std::size_t, EntryHeaderType>;
    using TableType = std::map<std::size_t, std::map<std::size_t, EntryType>>;
    enum TypesOfError : std::uint8_t {
      unexpectedSymbol = 0,
      expectedSymbol = 1,

    };

  private:
    Grammar &grammar;
    Logger &log;

    TableType table;

  public:
    SyntaxAnalyser(Grammar &, Logger &);
    SyntaxAnalyser(const SyntaxAnalyser &s) noexcept
        : grammar{s.grammar}, log{s.log} {}

    void print() const;
    void printTable() const;

    friend AST operator|([[maybe_unused]] Generator<Token> tokens,
                         [[maybe_unused]] SyntaxAnalyser &s) {
      using StackElement = std::pair<std::size_t, std::size_t>;
      using StackType = std::vector<StackElement>;

      StackType stack{StackElement{Grammar::DOLLAR_INDEX, 0}};

      std::vector<AST> astStack;
      std::vector<Token> tokenStack;
      
      auto symbols = s.grammar.getSymbols();
      auto rules = s.grammar.getRules();

      for (auto token : tokens) {
      checkpoint:
        //std::cout<<token<<std::endl;
        auto currentSymbol = token.getSymbol();
        auto [stackElement, currentState] = stack.back();
        if (symbols[currentSymbol].isTerminal()) {
          auto [id, act] = s.table[currentState][currentSymbol];
          if (act == EntryHeaderType::SHIFT) {
            //std::cout << "shift" << id << std::endl;
            stack.push_back(StackElement{currentSymbol, id});
            tokenStack.push_back(token);
          } else if (act == EntryHeaderType::REDUCE) {
            //std::cout << "reduce" << id << std::endl;
            auto rule = rules[id];
            //std::cout << "rule : "<<rule << std::endl;
            AST ast{id};
            auto v1 = rule.getRight();
            std::ranges::reverse(v1);
            //std::cout<<"size : "<<v1.size()<<std::endl;
            if(v1[0] != Grammar::EPSILON_INDEX){
              for (auto r : v1) {
                //std::cout<<"Poping...."<<std::endl;
                stack.pop_back();
                if (symbols[r].isNonterminal()) {
                  auto child = astStack.back();
                  astStack.pop_back();
                  ast.add(child);
                } else {
                  auto child = tokenStack.back();
                  tokenStack.pop_back();
                  ast.add(child);
                }
              }
            }
            astStack.push_back(ast);
            auto [ts,th]=stack.back();
            auto [ns,nh]=s.table[th][rule.getLeft()];
            //std::cout<<ns<<std::endl;

            stack.push_back(StackElement{rule.getLeft(), ns});
            goto checkpoint;
          } else if (act == EntryHeaderType::ACCEPT) {
            //std::cout<<"accepted"<<std::endl;
            auto rule = rules[id];
            AST ast{id};
            auto v = rule.getRight();
            std::ranges::reverse(v);
            for (auto r : v) {
              if (symbols[r].isNonterminal()) {
                auto child = astStack.back();
                astStack.pop_back();
                ast.add(child);
              } else {
                auto child = tokenStack.back();
                tokenStack.pop_back();
                ast.add(child);
              }
            }
            astStack.push_back(ast);
            break;
          } else {
            if (id-100 == unexpectedSymbol) {
              s.log(Error) << CompilerError::unexpectedSymbol(token) << lend;
            }
            else if (id/100 == expectedSymbol)
            {
              auto sym = id %100;
              s.log(Error) << CompilerError::expectedSymbol(token,symbols[sym]) << lend;
              token = Token{sym,"",token.getLocation()};
              goto checkpoint;
            }
          }
        }
        else{

        }
      }

      auto currentSymbol = Grammar::DOLLAR_INDEX;
      while(stack.size()>0){
        auto [stackElement, currentState] = stack.back();
        auto [id, act] = s.table[currentState][currentSymbol];
        if (act == EntryHeaderType::REDUCE) {
          //std::cout << "reduce" << id << std::endl;
          auto rule = rules[id];
          //std::cout << "rule : " << rule << std::endl;
          AST ast{id};
          auto v1 = rule.getRight();
          std::ranges::reverse(v1);
          //std::cout << "size : " << v1.size() << std::endl;
          if (v1[0] != Grammar::EPSILON_INDEX) {
            for (auto r : v1) {
              //std::cout << "Poping...." << std::endl;
              stack.pop_back();
              if (symbols[r].isNonterminal()) {
                auto child = astStack.back();
                astStack.pop_back();
                ast.add(child);
              } else {
                auto child = tokenStack.back();
                tokenStack.pop_back();
                ast.add(child);
              }
            }
          }
          astStack.push_back(ast);
          auto [ts, th] = stack.back();
          auto [ns, nh] = s.table[th][rule.getLeft()];
          //std::cout << ns << std::endl;
          stack.push_back(StackElement{rule.getLeft(), ns});
        } else if (act == EntryHeaderType::ACCEPT) {
          //std::cout << "accepted" << std::endl;
          auto rule = rules[id];
          AST ast{id};
          auto v = rule.getRight();
          std::ranges::reverse(v);
          for (auto r : v) {
            if (symbols[r].isNonterminal()) {
              auto child = astStack.back();
              astStack.pop_back();
              ast.add(child);
            } else {
              auto child = tokenStack.back();
              tokenStack.pop_back();
              ast.add(child);
            }
          }
          astStack.push_back(ast);
          break;
        }
      }
      return astStack[0];
    }
  };

  class SemanticAnalyser{
    private:
      
    public:
      friend SymbolTable operator|(AST,SemanticAnalyser){
        return {};
      }
  };

private:
  SyntaxAnalyser syntaxAnalyser;

public:
  Parser(Grammar &g, Logger &l);
  Parser(const Parser &p);

  void print() const;

  friend AST operator|(Generator<Token>, Parser &);


  template <typename t> void operator()(t x) { func = x; }

  ASTResult operator()(int r, Parser &p){
    return func(r,p);
  }
};

std::ostream &operator<<(std::ostream &o, Parser::SyntaxAnalyser::EntryType);

#endif // __PARSER_H__