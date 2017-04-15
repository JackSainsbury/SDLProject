#ifndef BLOCKDATABASE_H
#define BLOCKDATABASE_H

#include <string>
#include <unordered_map>

#include "Block.h"

class BlockDatabase
{
public:
    BlockDatabase(std::string _DatabaseFileDirectory);
    void loadBlockDatabase();

    inline std::string GetDatabaseFileDirectory(){ return m_DatabaseFileDirectory; }

    inline Block* GetBlockFromDirectory(int _id){ return &*m_blocks[_id];}
private:
    std::string m_DatabaseFileDirectory;

    std::unordered_map<int, std::unique_ptr<Block>> m_blocks;
};

#endif
