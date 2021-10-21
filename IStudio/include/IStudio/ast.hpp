#ifndef DBB36493_6408_4E5C_9C3F_F51BC090D435
#define DBB36493_6408_4E5C_9C3F_F51BC090D435

#include <IStudio/exception.hpp>
#include <IStudio/token.hpp>
#include <IStudio/types.hpp>

class Parser;

class AST {

  std::size_t ruleIndex;
  std::vector<Token> tokens;
  std::vector<AST> childAST;
  

public:
  AST(std::size_t r):ruleIndex{r}{}
  AST(std::size_t r,std::vector<AST>&& childs) : ruleIndex{r},childAST{std::move(childs)} {}
  friend std::ostream &operator<<(std::ostream &o, AST a) {
    o << a.ruleIndex;
    return o;
  }

  void add(AST child){
    childAST.push_back(child);
  }

  void add(Token token){
    tokens.push_back(token);
  }

  auto getChilds()const{
    return childAST;
  }

  auto operator[](size_t n)const{
    if(n-1<childAST.size())
      return childAST[n-1];
    else
      throw GrammarError{"AST don't contain this child"};
  }
};

#endif /* DBB36493_6408_4E5C_9C3F_F51BC090D435 */
