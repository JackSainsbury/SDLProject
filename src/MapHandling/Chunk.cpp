#include "Chunk.h"
#include <ngl/Random.h>
#include <sstream>
#include "MapHandler.h"


Chunk::Chunk()
{

}

void Chunk::SetMap(MapHandler *_map){
    m_map = _map;
}

//populate should only ever be called when making a new file, so write to file afterwards
void Chunk::PopulateChunk(){
    // grab an instance of the random generator
    ngl::Random *rng=ngl::Random::instance();

    for(int i = 0; i < m_map->GetChunkDimension(); ++i){
        std::vector<int> column;
        for(int j = 0; j < m_map->GetChunkDimension(); ++j){
            ngl::Real _weighting = rng->randomPositiveNumber();
            if(_weighting < .85)
                column.push_back(1); //set values to 1 (0 is mined, 1 is filled)
            else if(_weighting < .95)
                column.push_back(2); //set 2 (ore type 2)
            else
                column.push_back(3); //set 3 (ore type 1)
        }
        BlockVector.push_back(column);
    }

    //make the new file, then close it for safe measure
    std::ofstream newFile;
    newFile.open (curFileName);
    newFile.close();

    WriteChunkToFile();
}

//Write to file
void Chunk::WriteChunkToFile(){
    std::ifstream ifs(curFileName);

    if(ifs.good()){
        std::ofstream curFile;
        curFile.open (curFileName);
        if(curFile.is_open()){ //little safety net (probably unecessary but not many chunks to check)
            std::string line;
            for(int i = 0; i < m_map->GetChunkDimension(); ++i){
                for(int j = 0; j < m_map->GetChunkDimension(); ++j)
                {
                    line.append(std::to_string(BlockVector[i][j]) + ";");
                }
                curFile << line <<"\n";
                //write to file
                line.clear();
            }

            curFile.close();
        }
    }
}

//Read from file
void Chunk::ReadChunkFromFile(std::string _fname){

    if(BlockVector.empty()){
        for(int i = 0; i < m_map->GetChunkDimension(); ++i){
            std::vector<int> column(m_map->GetChunkDimension(), 0 );

            BlockVector.push_back(column);
        }
    }
    std::string line;
    std::ifstream myfile (curFileName);
    if (myfile.is_open())
    {
        int lineCounter = 0;
        while ( getline (myfile,line) )
        {
            for(int j = 0; j < m_map->GetChunkDimension(); ++j){
                std::istringstream split(line);
                std::vector<std::string> candidates;
                for (std::string each; std::getline(split, each, ';'); candidates.push_back(each));
                BlockVector[lineCounter][j] = stoi(candidates[j]); //IF THE MAP IS SAVING I , J BUT LOADING J , I THIS IS WHERE YOU MESSED UP <3 YOU'RE WELCOME FUTURE JACK
            }
            lineCounter++;
        }
        myfile.close();
    }
}
