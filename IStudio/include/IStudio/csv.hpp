#ifndef A92AB55E_B825_40D6_989F_6163FF75A63A
#define A92AB55E_B825_40D6_989F_6163FF75A63A

#include<IStudio/types.hpp>

class CSVReader {
private:
  std::map<std::size_t, std::map<std::string, std::string>> table;
  std::vector<std::string> header;

public:
  CSVReader(std::string name) {
    std::ifstream file{name};
    std::string line;
    std::getline(file, line);
    line = line.substr(0,line.size()-1);
    for (auto col : split(trim(line), ","))
      header.push_back(trim(col));

    while (std::getline(file, line)) {
      if(line=="\r")
        continue;
      line = line.substr(0, line.size() - 1);
      auto values = split(line, ",");
      // std::cout << values.size() << std::endl;
      // std::cout << header.size() << std::endl;
      assert(values.size()==header.size(),"This must be equal",std::source_location::current());
      for (std::size_t i = 1; i < header.size(); i++){
        //std::cout<<trim(values[0])<<" "<<header[i]<<" "<<trim(values[i])<<std::endl;
        table[stoi(trim(values[0]))][header[i]] = values[i];
      }
    }
  }
  auto getTable(){
      return table;
  }
};

#endif /* A92AB55E_B825_40D6_989F_6163FF75A63A */
