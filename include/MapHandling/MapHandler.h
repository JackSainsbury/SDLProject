#ifndef MAPHANDLER_H
#define MAPHANDLER_H

#include <ngl/VAOPrimitives.h>
#include <vector> //included in VAOPrim but kept for future proof VAO redundancy
#include <memory> //"
#include "Chunk.h"
#include <ngl/Vec2.h>

namespace MapHandlerCycleSpace{
    enum Direction{
        North,  //0
        East,   //1
        South,  //2
        West    //3
    };
}

class MapHandler
{
public:
    MapHandler(std::string _mapName, int _chunkLoadDimension, int _chunkDimension);

    inline Chunk GetChunk(int _x, int _y){ return *LoadedChunks[_x][_y]; }

    inline int GetChunkVectorDimension(){ return ChunkVectorDimension; }
    inline int GetChunkDimension(){ return ChunkDimension; }

    void initMap();
    void cyclePushChunks(MapHandlerCycleSpace::Direction _dir);

    ngl::Vec2 currentOfsetVector;//MAKE PRIVATE WHEN FIXED

private:
    std::vector<std::vector<std::unique_ptr<Chunk>>> LoadedChunks;
    void DoCycle(int _dir);

    int ChunkVectorDimension;
    int ChunkDimension;



    std::string curMapToLoad;
};

#endif // MAPHANDLER_H
