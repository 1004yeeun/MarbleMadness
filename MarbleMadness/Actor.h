#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* sw, int imageID, int startX, int startY, int startDir=0) : GraphObject(imageID, startX, startY, startDir), m_sw(sw), m_isAlive(true) {}
    
    virtual void doSomething() = 0;
    virtual bool allowsAgentColocation() = 0;
    
    virtual bool allowsMarble() { return false; }
    virtual bool countsInFactoryCensus() { return false; }
    virtual bool isSwallowable() { return false; }
    virtual bool isStealable() { return false; }
    virtual bool isDamageable() { return false; }
    virtual bool canDie() { return false; }
    virtual bool stopsPeas() { return false; }
    
    StudentWorld* getWorld() { return m_sw; }
    bool isAlive() { return m_isAlive; }
    void setDead() { m_isAlive = false; }
private:
    StudentWorld* m_sw;
    bool m_isAlive;
};

class Wall : public Actor {
public:
    Wall(StudentWorld* sw, int startX, int startY) : Actor(sw, IID_WALL, startX, startY, none) {}
    
    virtual void doSomething() { return; }
    virtual bool allowsAgentColocation() { return false; }
    virtual bool stopsPeas() { return true; }
};

class Agent : public Actor {
public:
    Agent(StudentWorld* sw, int imageID, int startX, int startY) : Actor(sw, imageID, startX, startY, right), m_hP(1) {}
    
    virtual bool allowsAgentColocation() { return false; }
    virtual bool stopsPeas() { return true; }
    virtual bool isDamageable() { return true; }
    virtual bool canDie() { return true; }
    void setHealth(int num) { m_hP = num; }
    void loseHealth() {m_hP--; }
    int getHealth() { return m_hP; }
    bool canMove(int dir);
    void newPos(int dir, int& newX, int& newY);
    bool move(int dir);
private:
    int m_hP;
};

class Avatar : public Agent {
public:
    Avatar(StudentWorld* sw, int startX, int startY) : Agent(sw, IID_PLAYER, startX, startY), m_peas(20) {}
    
    virtual void doSomething();
    
    void incPeas(int amt) { m_peas+=amt; }
    void decPea() { m_peas--; }
    int getPeas() { return m_peas; }
private:
    int m_peas;
};

#endif // ACTOR_H_
