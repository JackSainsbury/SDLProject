#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <iostream>
#include <fstream>

class MapHandler;

class Chunk
{
public:
    Chunk();

    void SetMap(MapHandler *_map);

    //inline void SetBlock ( int _x, int _y, int _blockID){ BlockVector[_x][_y] = _blockID; }
    inline int GetBlock ( int _x, int _y ) { return BlockVector[_x][_y]; }

    void PopulateChunk();

    void WriteChunkToFile();
    void ReadChunkFromFile(std::string _fname);

    inline void SetCurFileName( std::string _fname ) { curFileName = _fname; }
    inline std::string GetCurFileName() { return curFileName; }

private:
    std::vector<std::vector<int>> BlockVector;
    std::string curFileName;
    MapHandler *m_map;
};

#endif // CHUNK_H
