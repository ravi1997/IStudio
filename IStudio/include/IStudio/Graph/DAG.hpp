#ifndef D67D4B1A_1AFA_47D4_85BC_F58359BEB150
#define D67D4B1A_1AFA_47D4_85BC_F58359BEB150

#include "..//types.hpp"
#include "..//token.hpp"
#include <memory>

namespace details {
class DAG{
private:
    class DagNode{
        std::shared_ptr<DagNode> right;
        std::shared_ptr<DagNode> left;
        std::shared_ptr<DagNode> parent;
        Token value;
    };
    std::shared_ptr<DagNode> root = nullptr;

public:
    DAG()=default;
    DAG(const DAG& d):root{d.root}{}
    DAG& operator=(const DAG& d){
        root = d.root;
        return *this;
    }

    
};
}

#endif /* D67D4B1A_1AFA_47D4_85BC_F58359BEB150 */
