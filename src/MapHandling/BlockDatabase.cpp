#include "BlockDatabase.h"

BlockDatabase::BlockDatabase(std::string _DatabaseFileDirectory){
    m_DatabaseFileDirectory = _DatabaseFileDirectory;
    //Get all information about blocks from database file
    loadBlockDatabase();
}

//Load from file
void BlockDatabase::loadBlockDatabase(){

    std::string line;
    std::ifstream myfile (GetDatabaseFileDirectory());
    //My open Database
    if (myfile.is_open())
    {
        int LineCounter = 0;
        while ( getline (myfile,line) )
        {
            m_blocks[LineCounter] = std::unique_ptr<Block>(new Block(line));
            LineCounter++;
        }
        myfile.close();
    }

    //Block* test = &*m_blocks[1];
}
