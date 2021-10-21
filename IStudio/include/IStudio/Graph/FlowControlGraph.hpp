#ifndef E05B12FE_0989_43FF_8525_E69A88C4207B
#define E05B12FE_0989_43FF_8525_E69A88C4207B

#include "IStudio/types.hpp"
#include <IStudio/Graph/Block.hpp>

class FlowControlGraph{
public:
    using linkType = std::pair<size_t,size_t>;
private:
    std::vector<details::Block> blocks;
    std::vector<linkType> links;
public:

};


#endif /* E05B12FE_0989_43FF_8525_E69A88C4207B */
