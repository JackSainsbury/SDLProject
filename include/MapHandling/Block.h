#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>

#include "Entity.h"

class Block
{
public:
    Block(std::string _BlockName);

    inline std::string GetBlockName(){ return m_blockName; }

    void OnDestroy();
    void OnPlace();

private:
    std::string m_blockName;
    std::vector<Entity> m_dropObjects;
};

#endif
