#ifndef A2D5B12C_E2BE_47FC_80DE_99C0FA9B8B09
#define A2D5B12C_E2BE_47FC_80DE_99C0FA9B8B09

#include <IStudio/types.hpp>
#include <IStudio/Graph/DAG.hpp>

namespace details {
class Block{
private:
    std::size_t id;
    std::vector<details::DAG> DAGs;
public:
    auto getID()const{
        return id;
    }
    auto getDAGs()const{
        return DAGs;
    }
};
}

#endif /* A2D5B12C_E2BE_47FC_80DE_99C0FA9B8B09 */
