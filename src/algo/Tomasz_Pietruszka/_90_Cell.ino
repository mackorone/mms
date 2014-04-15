#include "Cell.h"
#include "StandardCplusplus.h"

#define NULL 0

Cell::Cell() : m_x(0), m_y(0), m_distance(0), m_explored(false), m_traversed(false), m_prev(NULL){
    /*for (int i = 0; i < 4; i++){
        m_walls[i] = false;
        m_wallsInspected[i] = false;
    }*/
    m_wall_and_inspected = 0;
}

Cell::~Cell()
{ }

int Cell::getX(){
    return m_x;
}

int Cell::getY(){
    return m_y;
}

int Cell::getDistance(){
    return m_distance;
}

bool Cell::getExplored(){
    return m_explored;
}

bool Cell::getTraversed(){
    return m_traversed;
}

Cell* Cell::getPrev(){
    return m_prev;
}

bool Cell::isWall(int direction){
  switch(direction) {
        case 0:
            return (bool) (m_wall_and_inspected & 1);
        case 1:
            return (bool) (m_wall_and_inspected & 2);
        case 2:
            return (bool) (m_wall_and_inspected & 4);
        case 3:
            return (bool) (m_wall_and_inspected & 8);
      }  
  
  //return m_walls[direction];
}

bool Cell::getWallInspected(int direction){
  
    switch(direction) {
        case 0:
            return (bool) (m_wall_and_inspected & 16);
        case 1:
            return (bool) (m_wall_and_inspected & 32);
        case 2:
            return (bool) (m_wall_and_inspected & 64);
        case 3:
            return (bool) (m_wall_and_inspected & 128);
      }  
   // return m_wallsInspected[direction];
}

void Cell::setX(int x){
    m_x = (unsigned char) x;
}

void Cell::setY(int y){
    m_y = (unsigned char) y;
}

void Cell::setDistance(int distance){
    m_distance = (unsigned char) distance;
}

void Cell::setExplored(bool explored){
    m_explored = explored;
}

void Cell::setTraversed(bool traversed){
    m_traversed = traversed;
}

void Cell::setWall(int direction, bool isWall){
    if (isWall) {
      switch (direction){
        case 0:
            m_wall_and_inspected |= 1;
            break;
        case 1:
            m_wall_and_inspected |= 2;
            break;
        case 2:
            m_wall_and_inspected |= 4;
            break;
        case 3:
            m_wall_and_inspected |= 8;
            break;
      }
    }
    else{
      switch (direction){
        case 0:
            m_wall_and_inspected &= 254;
            break;
        case 1:
            m_wall_and_inspected &= 253;
            break;
        case 2:
            m_wall_and_inspected &= 251;
            break;
        case 3:
            m_wall_and_inspected &= 247;
            break;
      }
    }
    //m_walls[direction] = isWall;
}

void Cell::setWallInspected(int direction, bool inspected){
      if (inspected) {
      switch (direction){
        case 0:
            m_wall_and_inspected |= 16;
            break;
        case 1:
            m_wall_and_inspected |= 32;
            break;
        case 2:
            m_wall_and_inspected |= 64;
            break;
        case 3:
            m_wall_and_inspected |= 128;
            break;
      }
    }
    else{
      switch (direction){
        case 0:
            m_wall_and_inspected &= 239;
            break;
        case 1:
            m_wall_and_inspected &= 223;
            break;
        case 2:
            m_wall_and_inspected &= 191;
            break;
        case 3:
            m_wall_and_inspected &= 127;
            break;
      }
    }
    //m_wallsInspected[direction] = inspected;
}

void Cell::setPrev(Cell* prev){
    m_prev = prev;
}
