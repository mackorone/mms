#include "Maze.h"

#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>
#include <time.h>
#include <vector>

#include "Constants.h"
#include "Parameters.h"
#include "Tile.h"

// TODO: Change operator[] to .at() in all locations in this file

Maze::Maze(int width, int height, std::string mazeFile){

    // Initialize the tile positions
    for (int i = 0; i < width; i++){
        std::vector<Tile> col;
        for (int j = 0; j < height; j++){
            Tile tile;
            tile.setPos(i, j);
            col.push_back(tile);
        }
        m_maze.push_back(col);
    }

    // Initialize the tile wall values
    if (mazeFile == ""){
        randomize();
        saveMaze("mazeFiles/auto_generated_maze.maz"); 
    }
    else{
        loadMaze(mazeFile);
    }
}

Maze::~Maze()
{ }

int Maze::getWidth(){
    return m_maze.size();
}

int Maze::getHeight(){
    if (m_maze.size() > 0){
        return m_maze.at(0).size();
    }
    return 0;
}

Tile* Maze::getTile(int xPos, int yPos){
    return &m_maze.at(xPos).at(yPos);
}

void Maze::randomize(){
    
    // Declare width and height locally to reduce function calls
    int width = getWidth();
    int height = getHeight();

    // Seed the random number generator
    srand(time(NULL));
    
    // The probability of any one wall appearing is 1/probDenom
    int probDenom = 3;

    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            bool walls[4]; // Make a walls array for tile (i, j)
            for (int k = 0; k < 4; k++){
                walls[k] = rand() % probDenom == 0;
                switch (k){
                    case NORTH:
                        if (j + 1 < height){
                            m_maze[i][j+1].setWall(SOUTH, walls[k]);
                            m_maze[i][j].setWall(NORTH, walls[k]);
                        }
                        else {
                            m_maze[i][j].setWall(NORTH, true);
                        }
                        break;
                    case EAST:
                        if (i + 1 < width){
                            m_maze[i+1][j].setWall(WEST, walls[k]);
                            m_maze[i][j].setWall(EAST, walls[k]);
                        }
                        else {
                            m_maze[i][j].setWall(EAST, true);
                        }
                        break;
                    case SOUTH:
                        if (j > 0){
                            m_maze[i][j-1].setWall(NORTH, walls[k]);
                            m_maze[i][j].setWall(SOUTH, walls[k]);
                        }
                        else {
                            m_maze[i][j].setWall(SOUTH, true);
                        }
                        break;
                    case WEST:
                        if (i > 0){
                            m_maze[i-1][j].setWall(EAST, walls[k]);
                            m_maze[i][j].setWall(WEST, walls[k]);
                        }
                        else {
                            m_maze[i][j].setWall(WEST, true);
                        }
                        break;
                }
            }
        }
    }

    // Ensures that the middle is hallowed out
    if (width % 2 == 0){
        if (height % 2 == 0){
            m_maze[width/2-1][height/2-1].setWall(NORTH, false);
            m_maze[width/2-1][height/2].setWall(SOUTH, false);
            m_maze[width/2][height/2-1].setWall(NORTH, false);
            m_maze[width/2][height/2].setWall(SOUTH, false);
            m_maze[width/2-1][height/2-1].setWall(EAST, false);
            m_maze[width/2][height/2-1].setWall(WEST, false);
        }
        m_maze[width/2-1][height/2].setWall(EAST, false);
        m_maze[width/2][height/2].setWall(WEST, false);
            
    }
    if (height % 2 == 0){
        m_maze[width/2][height/2-1].setWall(NORTH, false);
        m_maze[width/2][height/2].setWall(SOUTH, false);
    }
}

void Maze::saveMaze(std::string mazeFile){
    
    // Create the stream
    std::ofstream file(mazeFile.c_str());

    if (file.is_open()){

        // Very primitive, but will work
        for (int i = 0; i <  getWidth(); i++){
            for (int j = 0; j < getHeight(); j++){
                file << i << " " << j;
                for (int k = 0; k < 4; k++){
                    file << " " << (m_maze[i][j].isWall(k) ? 1 : 0);
                }
                file << std::endl;
            }
        }

        file.close();
    }
}

void Maze::loadMaze(std::string mazeFile){

    // Create the stream
    std::ifstream file(mazeFile.c_str());

    // Initialize a string variable
    std::string line("");

    if (file.is_open()){

        // Very primitive, but will work
        while (getline(file, line)){
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                 std::back_inserter<std::vector<std::string> >(tokens));
            for (int i = 0; i < 4; i++){ // Set the values of all of the walls
                m_maze[atoi(tokens.at(0).c_str())][atoi(tokens.at(1).c_str())]
                      .setWall(i, atoi(tokens.at(2+i).c_str()));
            }
        }
    
        file.close();
    }
}
