#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <vector>
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Avatar;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld() { cleanUp(); }
    
    virtual bool addWall(int x, int y);
    bool isValidPos(int x, int y);
    bool isValidPos(int x, int y, Actor* actor);
    bool isLevelCompleted () { return m_levelCompleted; }
    void setLevelCompletion(bool hm) { m_levelCompleted = hm; }
private:
    Avatar* m_avatar;
    vector<Actor*> actors;
    bool m_levelCompleted;
};

#endif // STUDENTWORLD_H_
