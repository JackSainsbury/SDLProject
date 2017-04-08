#include "MapHandler.h"
#include <sys/stat.h>
#include <sys/types.h>

MapHandler::MapHandler(std::string _mapName, int _chunkLoadDimension, int _chunkDimension){
    curMapToLoad = _mapName;

    ChunkVectorDimension = _chunkLoadDimension;
    ChunkDimension = _chunkDimension;

    if ( ChunkVectorDimension % 2 == 0 )
    {
        ChunkVectorDimension ++;    //make certain odd
    }else if(ChunkDimension == 0 || ChunkVectorDimension == 0){
        ChunkVectorDimension = 5;   //Default value checks
        ChunkDimension = 16;        //Default value checks
    }
    initMap();
}
//one time on start up
void MapHandler::initMap(){

    mkdir(curMapToLoad.c_str(), 0777);

    LoadedChunks.clear();//clear chunks vectors (safety)

    for(int i =0; i < ChunkVectorDimension; ++i){
        std::vector<std::unique_ptr<Chunk>> column;

        for(int j =0; j < ChunkVectorDimension; ++j){
            std::string fName = (std::to_string(i - (ChunkVectorDimension-1) / 2) + ":" + std::to_string(j - (ChunkVectorDimension-1) / 2) + ".txt");

            //ifStream the "curMapToLoad" directory and the fname (fname being chunk based)
            std::ifstream ifs (curMapToLoad + "/" + fName);
            //init map is only ever called on the first generation of our chunk set, so new chunks to populate vector
            column.push_back( std::unique_ptr<Chunk>(new Chunk()));

            column[j]->SetMap(this);

            column[j]->SetCurFileName(curMapToLoad + "/" + fName);

            if (ifs.good()){
                //File exists, read it, populating with existing values
                column[j]->ReadChunkFromFile(fName);

            }else{
                //File doesn't exist, populate with new values (if chunk is deleted, a new file should* be regenerated)
                column[j]->PopulateChunk();
            }
        }
        LoadedChunks.push_back(std::move(column));
    }
}


void MapHandler::cyclePushChunks(MapHandlerCycleSpace::Direction _dir){



    switch(_dir){
    case 0:
        currentOfsetVector = ngl::Vec2(currentOfsetVector.m_x,currentOfsetVector.m_y-1);

        for(int i = 0; i < ChunkVectorDimension; ++i){
            LoadedChunks[i].insert(LoadedChunks[i].begin(), std::unique_ptr<Chunk>(new Chunk()));

            LoadedChunks[i].erase(LoadedChunks[i].end() -1);

            LoadedChunks[i][0]->SetMap(this); //this is the new chunk I've just created (per collumn)

            std::string fName = (std::to_string((i - (ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_x)) + ":" +
                                 std::to_string(-((ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_y)) + ".txt");


            LoadedChunks[i][0]->SetCurFileName(curMapToLoad + "/" + fName);

            for(int j = 0; j < ChunkVectorDimension; ++j)
                std::cout<<"| "<< LoadedChunks[i][j]->GetCurFileName()<<" |";
            std::cout<<std::endl;
            //ifStream the "curMapToLoad" directory and the fname (fname being chunk based)
            std::ifstream ifs (curMapToLoad + "/" + fName);

            if (ifs.good()){
                //File exists, read it, populating with existing values
                LoadedChunks[i][0]->ReadChunkFromFile(fName);

            }else{
                //File doesn't exist, populate with new values (if chunk is deleted, a new file should* be regenerated)
                LoadedChunks[i][0]->PopulateChunk();
            }
        }
        std::cout<<currentOfsetVector.m_x<<","<<currentOfsetVector.m_y;
        std::cout<<std::endl;
        break;
    case 1:
    {

        currentOfsetVector = ngl::Vec2(currentOfsetVector.m_x+1,currentOfsetVector.m_y);

        std::vector<std::unique_ptr<Chunk>> newRow;

        LoadedChunks[0].clear();

        LoadedChunks.erase(LoadedChunks.begin());

        for(int i = 0; i < ChunkVectorDimension; ++i){

            newRow.push_back(std::unique_ptr<Chunk>(new Chunk()));

            newRow[i]->SetMap(this); //this is the new chunk I've just created (per collumn)

            std::string fName = (std::to_string(((ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_x)) + ":" +
                                 std::to_string((i - (ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_y)) + ".txt");

            newRow[i]->SetCurFileName(curMapToLoad + "/" + fName);

            //ifStream the "curMapToLoad" directory and the fname (fname being chunk based)
            std::ifstream ifs (curMapToLoad + "/" + fName);

            if (ifs.good()){
                //File exists, read it, populating with existing values
                newRow[i]->ReadChunkFromFile(fName);

            }else{
                //File doesn't exist, populate with new values (if chunk is deleted, a new file should* be regenerated)
                newRow[i]->PopulateChunk();
            }
        }
        LoadedChunks.push_back(std::move(newRow));
    }

        break;
    case 2:
        currentOfsetVector = ngl::Vec2(currentOfsetVector.m_x,currentOfsetVector.m_y+1);

        for(int i = 0; i < ChunkVectorDimension; ++i){

            LoadedChunks[i].erase(LoadedChunks[i].begin());

            LoadedChunks[i].push_back(std::unique_ptr<Chunk>(new Chunk()));

            LoadedChunks[i][ChunkVectorDimension-1]->SetMap(this); //this is the new chunk I've just created (per collumn)

            std::string fName = (std::to_string((i - (ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_x)) + ":" +
                                 std::to_string(((ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_y)) + ".txt");

            LoadedChunks[i][ChunkVectorDimension-1]->SetCurFileName(curMapToLoad + "/" + fName);

            for(int j = 0; j < ChunkVectorDimension; ++j)
                std::cout<<"| "<< LoadedChunks[i][j]->GetCurFileName()<<" |";
            std::cout<<std::endl;


            //ifStream the "curMapToLoad" directory and the fname (fname being chunk based)
            std::ifstream ifs (curMapToLoad + "/" + fName);

            if (ifs.good()){
                //File exists, read it, populating with existing values
                LoadedChunks[i][ChunkVectorDimension-1]->ReadChunkFromFile(fName);

            }else{
                //File doesn't exist, populate with new values (if chunk is deleted, a new file should* be regenerated)
                LoadedChunks[i][ChunkVectorDimension-1]->PopulateChunk();
            }
        }
        std::cout<<currentOfsetVector.m_x<<","<<currentOfsetVector.m_y;
        std::cout<<std::endl;
        break;
    case 3:
    {

        currentOfsetVector = ngl::Vec2(currentOfsetVector.m_x-1,currentOfsetVector.m_y);

        std::vector<std::unique_ptr<Chunk>> newRow;

        LoadedChunks[ChunkVectorDimension-1].clear();

        LoadedChunks.erase(LoadedChunks.end()-1);

        for(int i = 0; i < ChunkVectorDimension; ++i){

            newRow.push_back(std::unique_ptr<Chunk>(new Chunk()));

            newRow[i]->SetMap(this); //this is the new chunk I've just created (per collumn)

            std::string fName = (std::to_string(-((ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_x)) + ":" +
                                 std::to_string((i - (ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_y)) + ".txt");

            newRow[i]->SetCurFileName(curMapToLoad + "/" + fName);

            //ifStream the "curMapToLoad" directory and the fname (fname being chunk based)
            std::ifstream ifs (curMapToLoad + "/" + fName);

            if (ifs.good()){
                //File exists, read it, populating with existing values
                newRow[i]->ReadChunkFromFile(fName);

            }else{
                //File doesn't exist, populate with new values (if chunk is deleted, a new file should* be regenerated)
                newRow[i]->PopulateChunk();
            }
        }
        LoadedChunks.insert(LoadedChunks.begin(),std::move(newRow));
    }
        break;
    }
}
