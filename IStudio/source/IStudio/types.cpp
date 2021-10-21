#ifndef __TYPES_H__
#define __TYPES_H__

#include<IStudio/types.hpp>


namespace details {

void replaceAll(std::string &str, const std::string &from,
                const std::string &to) {
  if (from.empty())
    return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // In case 'to' contains 'from', like replacing
                              // 'x' with 'yx'
  }
}

std::string readFile(std::string fileName) {
  std::ifstream inFile{fileName}; // open the input file

  std::stringstream strStream;
  strStream << inFile.rdbuf(); // read the file
  return strStream.str();
}
}
#endif // __TYPES_H__