#ifndef A3738E3B_B85E_4D15_9386_5B9249DF7D94
#define A3738E3B_B85E_4D15_9386_5B9249DF7D94
#pragma once

#include <CLI/cli.hpp>
#include <Location/Location.hpp>
#include <Logger/Logger.hpp>
#include <Property/property.hpp>
#include <SymbolTable/SymbolTable.hpp>
#include <Types/types.hpp>
#include <filesystem>
#include <regex>
namespace fs = std::filesystem;


using First = std::vector<std::size_t>;
using Follow = std::vector<std::size_t>;


class Symbol;
class Grammar;
class Item;

using State = std::vector<Item>;

using ASTResult = std::variant<bool, std::string>;

namespace std
{
  
std::string to_string(bool b) ;

}



namespace details {
void replaceAll(std::string &str, const std::string &from,
                const std::string &to);

std::string readFile(std::string fileName);
}

#endif /* A3738E3B_B85E_4D15_9386_5B9249DF7D94 */
