#ifndef FC22014D_3C05_48CE_B5ED_374614E2BBB6
#define FC22014D_3C05_48CE_B5ED_374614E2BBB6

#include "IStudio/types.hpp"
#include "IStudio/token.hpp"


namespace CompilerError {

    class CompilerError{
    private:
      std::uint32_t errorID;
      std::string name;
      std::string description;
      details::Location location;
    public:
      CompilerError(
            std::uint32_t ID, 
            std::string n,
            std::string desc, 
            details::Location loc):
            errorID{ID},
            name{n},
            description{desc},
            location{loc}
            {}
        CompilerError(const CompilerError& c):
            errorID{c.errorID},
            name{c.name},
            description{c.description},
            location{c.location}
            {}


        auto getLocation()const{
            return location;
        }

        auto getName()const{
            return name;
        }

        auto getDescription()const{
            return description;
        }

        auto getErrorID()const{
            return errorID;
        }
    };
    template <typename t>
    t operator<<(t log,CompilerError c) {
      log << c.getLocation() << " : " << c.getName() << "\n";
      log << c.getDescription() << "(" << c.getErrorID() << ")";
      return log;
    }

    template<typename t>
    class CompilerErrorHelper:public t{
        public:
            CompilerErrorHelper(t){}
    };

    const static CompilerErrorHelper unexpectedSymbol = [](Token t)->CompilerError{
        return CompilerError{1,"UNEXPECTED SYMBOL",std::string{"The compiler didn't expect "}+ std::to_string(t.getSymbol()),t.getLocation()};
    };
    
    const static CompilerErrorHelper expectedSymbol =
        [](Token t,Symbol s) -> CompilerError {
      return CompilerError{1, "EXPECTED SYMBOL",
                           std::string{"The compiler expected "} + s.getName() + std::string{" but got "} + 
                               std::to_string(t.getSymbol()),
                           t.getLocation()};
    };
}

#endif /* FC22014D_3C05_48CE_B5ED_374614E2BBB6 */
