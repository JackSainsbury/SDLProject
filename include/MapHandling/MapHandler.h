#ifndef MAPHANDLER_H
#define MAPHANDLER_H

#include <ngl/VAOPrimitives.h>
#include <ngl/Vec2.h>
#include <ngl/Transformation.h>
#include <ngl/Obj.h>

#include <unordered_map>
#include <vector> //included in VAOPrim but here futureproof in case ofVAO redundancy
#include <memory> //included in VAOPrim but here futureproof in case ofVAO redundancy

#include "Chunk.h"
#include "Entity.h"

namespace MapHandlerCycleSpace{
    enum Direction{
        North,  //0
        East,   //1
        South,  //2
        West    //3
    };
}

class NGLDraw;

class MapHandler
{
public:
    MapHandler(std::string _mapName, int _chunkLoadDimension, int _chunkDimension);

    //Initialize my map, this will start population and file handling, also set forward declared MAINDRAWLOOP
    void InitMap(bool newWorld, NGLDraw *_mainDrawLoop);

    //<Inlines>
    inline Chunk GetChunk(int _x, int _y){ return *LoadedChunks[_x][_y]; }  //return the chunk from the "loaded chunks" vector at index (x,y)
    inline int GetChunkVectorDimension(){ return ChunkVectorDimension; }    //return the chunk vector dimension - how many chunks am I buffering
    inline int GetChunkDimension(){ return ChunkDimension; }                //return the dimension of each chunk - how many blocks per chunk in this world
    //</Inlines>

    inline ngl::Vec2 GetCurrentOfsetVector(){ return currentOfsetVector; }


    //Scroll the loaded chunks in a "dir - Enum" for when player has walked to the end of a chunk
    void CyclePushChunks(MapHandlerCycleSpace::Direction _dir);

    //Load the meshes from files to my unordered map, key is ints for ease of comparison to map files
    void LoadMineMeshes();

    void UpdateMapViaPlayerInput();

    void DrawMap();//draw my map from the loaded chunks and meshes, apply my global transform also (for inverse of "player movement" - world moves around the player who remains at 0,y?,0)

    inline ngl::Transformation* GetTransform(){return &m_transform;}

private:
    std::vector<std::vector<std::unique_ptr<Chunk>>> LoadedChunks;
    void DoCycle(int _dir);

    int ChunkVectorDimension;
    int ChunkDimension;

    std::string curMapToLoad;
    ngl::Vec2 currentOfsetVector;

    NGLDraw* MainDrawLoop;

    std::vector<std::unique_ptr<ngl::Obj>> MineMeshes;

    ngl::Transformation m_transform;
};

#endif // MAPHANDLER_H
