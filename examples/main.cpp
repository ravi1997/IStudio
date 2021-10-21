#include <CLI/CLIElement.hpp>
#include <CLI/cli.hpp>
#include <IStudio/ast.hpp>
#include <IStudio/exception.hpp>
#include <IStudio/grammar.hpp>
#include <IStudio/parser.hpp>
#include <IStudio/tokenizer.hpp>
#include <IStudio/Manifest/manifest.hpp>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
using namespace std;

struct CommandOptions {
  string command = "";
  string creationType = "";
  string compileType = "";
  string projectName = "";
  string moduleName = "";
  string valueName = "";
  string value = "";
  string dependencyName = "";
  bool debug = false;
  bool logFile = false;
};

bool ends_with(std::string value, std::string ending) {
  if (ending.size() > value.size())
    return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

string FiletoString(string f) {
  ifstream file{f};
  string result = "";

  getline(file, result, '\0');
  file.close();

  return result;
}

void functionCompile(string filename, Logger &log) {
  string grammar = FiletoString("FunctionRules");
  // cout<<grammar<<endl;
  Grammar g{grammar, log};
  string input = FiletoString(filename);
  input = trim(input);

  auto addSymbol = [&g](string fileName) {
    string symbols = FiletoString(fileName);
    for(auto symbol : std::split(symbols,"\n")){
      auto s = std::split(symbol,"$");
      auto name = s[0];
      auto pattern = s[1];
      std::transform(name.begin(), name.end(), name.begin(),
                     [](unsigned char c) { return std::tolower(c); });
      if(s.size()==3){
        // cout<<name<<endl;
        g.getSymbolByName(name).setAsOperator();
      }
      g.getSymbolByName(name).setPattern(pattern);
    }
  };
  
  addSymbol("Symbols"); 

  Parser p{g, log};
  p([](int r, Parser &) -> ASTResult {
    if (r == 0) {

    } else if (r == 1) {

    } else {
    }
    return {};
  });
  // p.print();

  Symbol space{"space"};
  space.setPattern("\\s");

  Symbol newLine{"newline"};
  newLine.setPattern("\r\n|\r|\n");
  vector skipSymbols = {space,newLine};

  //cout<<skipSymbols.size()<<endl;

  //assert(input[1]=='m');

  [[maybe_unused]] auto ast = input | Tokenizer{g.getSymbols(), skipSymbols} | p;
  cout << filename + " : accepted" << endl;

}

int main(int argc, char **argv) {
  const string configFilePath = "data/defaultConfig.txt";
  const string compilerVersion = "1.0.1";

  CommandOptions options;

  auto getDate = []() -> string {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
  };

  CLI::Cli cli{"IStudio",
               CLI::description =
                   "This is Cummulative package that contains many things.",
               CLI::version = "1.0.0.1", CLI::position = true};

  string authorName;
  string defaultFlags;
  string defaultProjectPhase;

  cli = Argument("Command", options.command)("This contains many Types")(
      {details::SubCLI{"create",
                       CLI::description =
                           "It helps to create various project or modules"} |
           Argument("CreationType", options.creationType)(
               "This will take the type of creation")(
               {details ::SubCLI{"project",
                                 CLI::description = "It will create project"} |
                    Argument("name", options.projectName)(
                        "This will be the project name"),
                details::SubCLI{"module",
                                CLI::description = "It will create module"} |
                    Argument("name", options.moduleName)(
                        "This will be the module name")}),
       details::SubCLI{"install",
                       CLI::description =
                           "It helps to install various project or libraries"} |
           Argument("dependencyName",
                    options.dependencyName)("This will be name of dependency"),
       details::SubCLI{"compile",
                       CLI::description = "It will compile the project"} |
           Argument("CompileType",
                    options.compileType)("it will take the type of compile")(
               {details ::SubCLI{"project",
                                 CLI::description = "It will create project"} |
                    Argument("name", options.projectName)(
                        "This will be the project name"),
                details::SubCLI{"module",
                                CLI::description = "It will create module"} |
                    Argument("name", options.moduleName)(
                        "This will be the module name")})});

  cli.parse(argc, argv);

  //try {
    if (options.command == "create") {
      if (options.creationType == "project") {
        if (fs::exists(options.projectName)) {
          throw string{"Project Already exists"};
        }

        ifstream configFile{configFilePath};
        string line = "";

        while (getline(configFile, line)) {
          if (line == "DEFAULT_USER_NAME") {
            getline(configFile, line);
            authorName = line;
          } else if (line == "DEFAULT_FLAGS") {
            getline(configFile, line);
            defaultFlags = "\"" + line + "\"";
          } else if (line == "DEFAULT_PROJECT_PHASE") {
            getline(configFile, line);
            defaultProjectPhase = "\"" + line + "\"";
          }
        }

        configFile.close();

        vector<string> directories{options.projectName,
                                   options.projectName + "/include",
                                   options.projectName + "/source",
                                   options.projectName + "/output",
                                   options.projectName + "/library",
                                   options.projectName + "/resource",
                                   options.projectName + "/resource/sound",
                                   options.projectName + "/resource/files",
                                   options.projectName + "/resource/images",
                                   options.projectName +
                                       "/resource/images/icon"};
        vector<string> files{options.projectName + "/source/" +
                                 options.projectName + ".iClass",
                             options.projectName + "/manefist.json"};

        for (auto directory : directories)
          fs::create_directories(directory);

        for (auto file : files) {
          ofstream temp{file, ios::out};
          temp.close();
        }

        string manefistContent;

        // code to read the file into manefistContent
        manefistContent = details::readFile("data/templateManefist.json");

        details::replaceAll(manefistContent, "{PROJECT_NAME}",
                            "\"" + options.projectName + "\"");
        details::replaceAll(manefistContent, "{MAIN_APPLICATION_FILE}",
                            "\"" + options.projectName + ".iClass\"");
        details::replaceAll(manefistContent, "{COMPILER_VERSION}",
                            "\"" + compilerVersion + "\"");
        details::replaceAll(manefistContent, "{CURRENT_DATE}",
                            "\"" + getDate() + "\"");
        details::replaceAll(manefistContent, "{DEFAULT_USER_NAME}",
                            "\"" + authorName + "\"");
        details::replaceAll(manefistContent, "{DEFAULT_FLAGS}", defaultFlags);
        details::replaceAll(manefistContent, "{DEFAULT_PROJECT_PHASE}",
                            defaultProjectPhase);
        ofstream manefist{options.projectName + "/manefist.json", ios::out};
        manefist << manefistContent;
        manefist.close();

        string mainFileContent;

        // code to read the file into manefistContent
        mainFileContent = details::readFile("data/templateMain.iClass");

        details::replaceAll(mainFileContent, "{PROJECT_NAME}",
                            options.projectName);
        details::replaceAll(mainFileContent, "{AUTHOR_NAME}", authorName);
        details::replaceAll(mainFileContent, "{CURRENT_DATE}",
                            "\"" + getDate() + "\"");
        details::replaceAll(mainFileContent, "{APPLICATION_NAME}",
                            options.projectName + "_Application");

        ofstream mainFile{options.projectName + "/source/" +
                              options.projectName + ".iClass",
                          ios::out};
        mainFile << mainFileContent;
        mainFile.close();

        // cout<<"ok"<<endl;

      } else if (options.creationType == "module") {
      } else {
      }
    } else if (options.command == "set") {
      // cout << "ok" << endl;
      if (options.valueName == "username")
        options.valueName = "DEFAULT_USER_NAME";
      ifstream configFile{configFilePath};
      ofstream temp{"data/temp", ios::out};
      string line = "";

      while (getline(configFile, line)) {
        line = line.substr(0, line.length() - 1);
        // cout<<(line == "DEFAULT_USER_NAME\r")<<"aa"<<line<<endl;
        if (line == options.valueName) {
          temp << line << endl;
          getline(configFile, line);
          temp << options.value << endl;
        } else {
          temp << line << endl;
          getline(configFile, line);
          line = line.substr(0, line.length() - 1);
          temp << line << endl;
        }
      }

      configFile.close();
      temp.close();

      rename("data/temp", configFilePath.c_str());
    } else if (options.command == "get") {
      if (options.valueName == "username")
        options.valueName = "DEFAULT_USER_NAME";

      ifstream configFile{configFilePath};
      string line = "";

      while (getline(configFile, line)) {
        if (line == options.valueName) {
          getline(configFile, line);
          cout << line << endl;
        } else
          getline(configFile, line);
      }
      configFile.close();
    } else if (options.command == "compile") {
      if (options.compileType == "project") {
        if (!fs::exists(options.projectName)) {
          std::cout << options.projectName <<std::endl;
          throw string{"Project Not Found"};
        }
        if (options.logFile) {
          if (!fs::exists(options.projectName + "/log")) {
            fs::create_directories(options.projectName + "/log");
            fs::create_directories(options.projectName + "/log/compile");

            vector<string> files{
                options.projectName + "/log/logfile.txt",
                options.projectName + "/log/table.txt",
                options.projectName + "/log/rules.txt",
                options.projectName + "/log/AST.txt",
                options.projectName + "/log/dependency.txt",
            };

            for (auto file : files) {
              ofstream temp{file, ios::out};
              temp.close();
            }
          }
        }
        Manifest manifest;
        json j;
        ifstream manifestFile{options.projectName + "/manefist.json"};
        manifestFile >> j;
        manifestFile.close();

        j.get_to(manifest);
        Logger log{"logfile.txt", level = 3, enable = options.logFile};

        log.If(level >= 0, Info)
            << "Compiler starting with setting defaults" << lend;
        log.If(level >= 0, Info) << "Grammar is being set" << lend;

        for (auto file : manifest.Files.source) {
          if (ends_with(file, ".ifunction")) {
            // cout << "ok" << endl;
            functionCompile(manifest.ProjectName+"/source/"+file, log);
          }
        }
      }
    }

  //} 
  // catch (GrammarError e) {
  //  cerr << e << endl;
  // }
  // catch (string s) {
  //  cerr << s << endl;
  // }

  // cout << "ok";
  return 0;
}
