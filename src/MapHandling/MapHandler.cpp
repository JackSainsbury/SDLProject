#include "MapHandler.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <NGLDraw.h>

MapHandler::MapHandler(std::string _mapName, int _chunkLoadDimension, int _chunkDimension){
    curMapToLoad = _mapName;

    ChunkVectorDimension = _chunkLoadDimension;
    ChunkDimension = _chunkDimension;

    if ( ChunkVectorDimension % 2 == 0 )
    {
        ChunkVectorDimension ++;    //make certain odd
    }else if(ChunkDimension < 2 || ChunkVectorDimension == 0){
        ChunkVectorDimension = 5;   //Default value checks
        ChunkDimension = 16;        //Default value checks
    }
}

//one time on start up
void MapHandler::InitMap(bool newWorld, NGLDraw *_mainDrawLoop){
    MainDrawLoop = _mainDrawLoop;

    LoadMineMeshes();

#if defined(_WIN32)
    _mkdir(curMapToLoad.c_str());
#else
    mkdir(curMapToLoad.c_str(), 0777);
#endif

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
    if(newWorld){
        if(ChunkDimension%2 == 0){
            for(int i = 0; i < 2; ++i){
                for(int j = 0; j < 2; ++j){
                    LoadedChunks[(ChunkVectorDimension-1)/2][(ChunkVectorDimension-1)/2]->SetBlock((ChunkDimension/2)-1+i,(ChunkDimension/2)-1+j,0);
                }
            }
        }else{
            for(int i = 0; i < 3; ++i){
                for(int j = 0; j < 3; ++j){
                    LoadedChunks[(ChunkVectorDimension-1)/2][(ChunkVectorDimension-1)/2]->SetBlock(((ChunkDimension-1)/2)-1+i,((ChunkDimension-1)-1/2)+j,0);
                }
            }
        }
        m_transform.addPosition(-(float(ChunkDimension-1)/2),0,-(float(ChunkDimension-1)/2));
    }
}

//This is a filthy switch case; consider refining. "NewRow()" for East & West and "NewCascadeRow()" for North and South (due to the 2d vector set up)
void MapHandler::CyclePushChunks(MapHandlerCycleSpace::Direction _dir){
    switch(_dir){
    case 0:
        currentOfsetVector = ngl::Vec2(currentOfsetVector.m_x,currentOfsetVector.m_y+1);

        for(int i = 0; i < ChunkVectorDimension; ++i){

            LoadedChunks[i].erase(LoadedChunks[i].begin());

            LoadedChunks[i].push_back(std::unique_ptr<Chunk>(new Chunk()));

            LoadedChunks[i][ChunkVectorDimension-1]->SetMap(this); //this is the new chunk I've just created (per collumn)

            std::string fName = (std::to_string((i - (ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_x)) + ":" +
                                 std::to_string(((ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_y)) + ".txt");

            LoadedChunks[i][ChunkVectorDimension-1]->SetCurFileName(curMapToLoad + "/" + fName);

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
        break;
    case 1:
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
    case 2:
        currentOfsetVector = ngl::Vec2(currentOfsetVector.m_x,currentOfsetVector.m_y-1);

        for(int i = 0; i < ChunkVectorDimension; ++i){
            LoadedChunks[i].insert(LoadedChunks[i].begin(), std::unique_ptr<Chunk>(new Chunk()));

            LoadedChunks[i].erase(LoadedChunks[i].end() -1);

            LoadedChunks[i][0]->SetMap(this); //this is the new chunk I've just created (per collumn)

            std::string fName = (std::to_string((i - (ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_x)) + ":" +
                                 std::to_string(-((ChunkVectorDimension-1) / 2) + int(currentOfsetVector.m_y)) + ".txt");


            LoadedChunks[i][0]->SetCurFileName(curMapToLoad + "/" + fName);

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
        break;
    case 3:
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
    }
}

//temporart till load all from directory function
void MapHandler::LoadMineMeshes(){
    MineMeshes.emplace_back(std::unique_ptr<ngl::Obj>(new ngl::Obj("Assets/models/Dirt.obj","Assets/textures/Dirt.png")));
    MineMeshes.emplace_back(std::unique_ptr<ngl::Obj>(new ngl::Obj("Assets/models/FlatGem.obj","Assets/textures/FlatGem.png")));
    MineMeshes.emplace_back(std::unique_ptr<ngl::Obj>(new ngl::Obj("Assets/models/TallGem.obj","Assets/textures/TallGem.png")));
    MineMeshes.emplace_back(std::unique_ptr<ngl::Obj>(new ngl::Obj("Assets/models/Chest.obj","Assets/textures/Chest.png")));

    for(int i = 0; i < MineMeshes.size(); ++i)
        MineMeshes[i]->createVAO();
}

void MapHandler::UpdateMapViaPlayerInput(){
    Player* _m_player = MainDrawLoop->GetPlayer();//Get values from my player

    //apply the inverse of the current player input vector via his movespeed to my position. (fram independent)
    m_transform.addPosition(_m_player->GetInputVector().m_x * _m_player->GetPlayerWalkSpeed() * (MainDrawLoop->deltaTimeMiliseconds/1000.0f)
                            ,0
                            ,_m_player->GetInputVector().m_y * _m_player->GetPlayerWalkSpeed() * (MainDrawLoop->deltaTimeMiliseconds/1000.0f));

    //CYLCING
    if(m_transform.getPosition().m_x>0.5){
        CyclePushChunks(MapHandlerCycleSpace::East);
        m_transform.setPosition(-(ChunkDimension-0.5),0,m_transform.getPosition().m_z);
    }else if(m_transform.getPosition().m_x < -(ChunkDimension-0.5)){
        CyclePushChunks(MapHandlerCycleSpace::West);
        m_transform.setPosition(0.5,0,m_transform.getPosition().m_z);
    }

    if(m_transform.getPosition().m_z>0.5){
        CyclePushChunks(MapHandlerCycleSpace::South);
        m_transform.setPosition(m_transform.getPosition().m_x,0,-(ChunkDimension-0.5));
    }else if(m_transform.getPosition().m_z < -(ChunkDimension-0.5)){
        CyclePushChunks(MapHandlerCycleSpace::North);
        m_transform.setPosition(m_transform.getPosition().m_x,0,0.5);
    }
}

void MapHandler::DrawMap(){
    UpdateMapViaPlayerInput();

    ngl::Transformation trans;//transformation of each block, set by chunk and chunk internal positions

    for(int i = 0; i<GetChunkVectorDimension();++i){
        for(int j = 0; j<GetChunkVectorDimension();++j){

            for(int chunkBlockX = 0; chunkBlockX<GetChunkDimension();++chunkBlockX){
                for(int chunkBlockY = 0; chunkBlockY<GetChunkDimension();++chunkBlockY){
                    trans.reset();

                    trans.setPosition((i - (GetChunkVectorDimension())/2)*GetChunkDimension() + chunkBlockX,1,
                                      (j - (GetChunkVectorDimension())/2)*GetChunkDimension() + chunkBlockY);

                    switch(GetChunk(i,j).GetBlock(chunkBlockX,chunkBlockY)){
                    case 1:
                        MainDrawLoop->loadMatricesToShader(trans.getMatrix() * m_transform.getMatrix());
                        // draw
                        MineMeshes[0]->draw();
                        break;
                    case 2:
                        //load
                        MainDrawLoop->loadMatricesToShader(trans.getMatrix() * m_transform.getMatrix());
                        // draw
                        MineMeshes[1]->draw();
                        break;
                    case 3:
                        //load
                        MainDrawLoop->loadMatricesToShader(trans.getMatrix() * m_transform.getMatrix());
                        // draw
                        MineMeshes[2]->draw();
                        break;
                    case 4:
                        //load
                        MainDrawLoop->loadMatricesToShader(trans.getMatrix() * m_transform.getMatrix());
                        // draw
                        MineMeshes[3]->draw();
                        break;
                    }
                }
            }
        }
    }
}
