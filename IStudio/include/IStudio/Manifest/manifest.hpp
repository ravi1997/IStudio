#ifndef DDD7A77A_09FE_405A_882D_F99282E8D58C
#define DDD7A77A_09FE_405A_882D_F99282E8D58C

#include <IStudio/Manifest/json.hpp>
#include <IStudio/types.hpp>
using namespace nlohmann;

struct files {
  std::vector<std::string> include;
  std::vector<std::string> source;
};

struct Library{
  std::vector<std::string> internal;
  std::vector<std::string> external;
};

struct Resource{
  std::vector<std::string> image;
  std::vector<std::string> files;
  std::vector<std::string> sounds;
};

struct Manifest {
    std::string ProjectName = "";
    std::string DateOfCreation = "";
    std::string ProjectVersion = "";
    std::string ProjectType;
    std::string ProjectPhase;
    std::string Author;
    files Files;
    Library libraries;
    Resource resource;
    //modules Modules;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Library, internal, external);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Resource, image, files, sounds);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(files, include, source);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Manifest, ProjectName, DateOfCreation,
                                   Files);


#endif /* DDD7A77A_09FE_405A_882D_F99282E8D58C */
