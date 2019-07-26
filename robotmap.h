#ifndef ROBOTMAP_H
#define ROBOTMAP_H

#include <vector>
#include <QDebug>
#include "robot.h"

#define TILE_EMPTY      0x0000
#define TILE_TOPWALL    0x0002
#define TILE_BOTTOMWALL 0x0004
#define TILE_LEFTWALL   0x0008
#define TILE_RIGHTWALL  0x0010

class RobotMap
{

   bool _isRobotAlive;

public:

    int** map;
    size_t w = 0, h = 0;
    Robot* robot;

    RobotMap() {
        robot = new Robot();
        _isRobotAlive = true;
    }

    int get(size_t i, size_t j) { return map[i][j]; }

    void setMap(std::vector<std::vector<int>> mapVector) {
        createMap(mapVector.size(), mapVector[0].size());
        for(size_t i = 0; i < h; i++) {
            for(size_t j = 0; j < w; j++) {
                map[i][j] = mapVector[i][j];
            }
        }
    }

    void setRobot(Robot* r) { robot = r; }

    void createMap(size_t w, size_t h) {

        this->w = w;
        this->h = h;

        map = new int*[h];
        for(size_t i = 0; i < h; i++) {
            map[i] = new int[w];
            for(size_t j = 0; j < w; j++) { map[i][j] = TILE_EMPTY; }
        }

    }

    void moveRobotTo(size_t i, size_t j) { robot->setMapPos(i, j); }

    void moveRobotUp() {
        if(!topIsFree()) { breakRobot(); }
        if(!isRobotAlive()) { return; }
        qDebug() << "up";
        robot->up();
    }
    void moveRobotDown()  {
        if(!bottomIsFree()) { breakRobot(); }
        if(!isRobotAlive()) { return; }
        qDebug() << "down";
        robot->down();
    }
    void moveRobotLeft()  {
        if(!leftIsFree()) { breakRobot(); }
        if(!isRobotAlive()) { return; }
        qDebug() << "left";
        robot->left();
    }
    void moveRobotRight() {
        if(!rightIsFree()) { breakRobot(); }
        if(!isRobotAlive()) { return; }
        qDebug() << "right";
        robot->right();
    }

    void breakRobot() { _isRobotAlive = false; }
    bool isRobotAlive() { return _isRobotAlive;  }

    bool topIsFree() { return topIsFree(robot->i, robot->j); }
    bool bottomIsFree() { return bottomIsFree(robot->i, robot->j); }
    bool leftIsFree() { return leftIsFree(robot->i, robot->j); }
    bool rightIsFree() { return rightIsFree(robot->i, robot->j); }

    bool topIsFree(size_t i, size_t j) {
        return isValid(i, j) && (map[i][j]&TILE_TOPWALL) == 0 &&
               isValid(i-1,j) && (map[i-1][j]&TILE_BOTTOMWALL) == 0;
    }

    bool bottomIsFree(size_t i, size_t j) {
        return isValid(i, j) && (map[i][j]&TILE_BOTTOMWALL) == 0 &&
                isValid(i+1, j) && (map[i+1][j]&TILE_TOPWALL) == 0;
    }

    bool leftIsFree(size_t i, size_t j) {
        return isValid(i, j) && (map[i][j]&TILE_LEFTWALL) == 0 &&
                isValid(i, j-1) && (map[i][j-1]&TILE_RIGHTWALL) == 0;
    }

    bool rightIsFree(size_t i, size_t j) {
        return isValid(i, j) && (map[i][j]&TILE_RIGHTWALL) == 0 &&
                isValid(i, j+1) && (map[i][j+1]&TILE_LEFTWALL) == 0;
    }

    bool isEmpty(size_t i, size_t j) { return map[i][j] == TILE_EMPTY; }
    bool isValid(size_t i, size_t j) { return i < h && j < w; }

    ~RobotMap() {
        //Free each sub-array
        for(size_t i = 0; i < h; ++i) { delete[] map[i]; }
        //Free the array of pointers
        delete[] map;
    }

};

#endif
