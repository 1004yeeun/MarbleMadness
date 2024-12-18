#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* sw, int imageID, int startX, int startY, int startDir=0) : GraphObject(imageID, startX, startY, startDir), m_sw(sw), m_isAlive(true), m_isStolen(false), m_hitPoints(1), m_hasStolenAGoodie(false), m_goodieType(0) {}
    virtual ~Actor() {}
    
    //mainly regarding marbles and agents
    void setHealth(int num) { m_hitPoints = num; } //initialize health at constr
    int getHealth() { return m_hitPoints; } //get current health
    virtual void loseHealth() { m_hitPoints -=2; }
    //decrease health (same for all but plays different sound)
    
    //if a pea colocates, will it be damaged?
    virtual bool isDamageable() { return false; }
    
    //if this object colocates another, can they steal it? (thiefbot indi)
    virtual bool canSteal() { return false;}
    void stoleAGoodie() { m_hasStolenAGoodie = true; }
    //if the actor stole a goodie, set true
    bool getStealingStatus() { return m_hasStolenAGoodie; }
    //return whether the actor is carrying a goodie or not
    int getGoodieType() { return m_goodieType; }
    //get an integer indicator of what goodie was stolen
    void setGoodieType(int goodieType) { m_goodieType = goodieType; }
    //set the m_goodieType to a specific goodieType indicator

    
    
    virtual void doSomething() = 0;
    virtual bool allowsAgentColocation() = 0;
    virtual bool allowsMarble() { return false; }
    virtual bool blocksBotRange() { return true; }
    virtual bool countsForFactoryCount() { return false; }
    virtual bool isSwallowable() { return false; }
    virtual bool isStealable() { return false; }
    virtual bool canDie() { return false; }
    virtual bool stopsPeas() { return false; }
    virtual bool canDieByBot() { return false; }
    virtual bool allowsMarbleColocation() { return false; }
    virtual bool givesLife() { return false; }
    virtual bool givesHealth() { return false; }
    virtual bool givesAmmo() { return false; }
    
    StudentWorld* getWorld() { return m_sw; }
    bool isAlive() { return m_isAlive; }
    virtual void setDead() { m_isAlive = false; }
    
    void setStolen() { m_isStolen = true; }
    bool isItStolen() { return m_isStolen; }
    
    double getHealthPercentage() {
        return (getHealth()/20.0) * 100;
    }
private:
    StudentWorld* m_sw;
    bool m_isAlive;
    int m_hitPoints;
    bool m_isStolen;
    bool m_hasStolenAGoodie;
    int m_goodieType;
};

class Agent : public Actor {
public:
    Agent(StudentWorld* sw, int imageID, int startX, int startY, int dir=0) : Actor(sw, imageID, startX, startY, dir) {}
    virtual ~Agent() {}
    
    //if a pea colocates, will it be damaged?
    virtual bool isDamageable() { return true; }

    
    //virtual bool canPush(int dir) = 0;
    
    virtual void pushPea(int dir) = 0;
    virtual bool checkPosForMarble(int dir) = 0;
    virtual bool needsClearShot() const { return false; }
    virtual bool allowsAgentColocation() { return false; }
    virtual bool canDie() { return true; }
    bool canMove(int dir);
    void newPos(int dir, int& newX, int& newY);
    bool move(int dir);
};

class Avatar : public Agent {
public:
    Avatar(StudentWorld* sw, int startX, int startY) : Agent(sw, IID_PLAYER, startX, startY), m_peas(20) {
        setHealth(20);
    }
    virtual ~Avatar() {}
    
    //use its peas to possibly attack something
    void incPeas(int amt) { m_peas+=amt; }
    void decPea() { m_peas--; }
    int getPeas() { return m_peas; }
    virtual void pushPea(int dir);
    //call sw to add a new pea actor to sw at x,y
    
    //loses health the same but plays a different sound
    virtual void loseHealth();
    
    
    
    
    
    virtual bool checkPosForMarble(int dir);
    virtual void doSomething();
    //virtual bool canPush(int dir);
    virtual bool blocksBotRange() { return false; }
    virtual bool canDieByBot() { return true; }
    virtual bool allowsMarble() { return true; }

private:
    int m_peas;
};

class Pea : public Actor {
public:
    Pea(StudentWorld* sw, int startX, int startY, int dir) : Actor(sw, IID_PEA, startX, startY, dir), m_goingToDie(false) {}
    virtual ~Pea() {}
    
    void goingToDieSoon() { m_goingToDie = true; }
    bool willItDie() { return m_goingToDie; }

    
    virtual void doSomething();
    virtual bool allowsAgentColocation() { return true; };
    virtual bool blocksBotRange() { return false; }
private:
    bool m_goingToDie;
};

class Wall : public Actor {
public:
    Wall(StudentWorld* sw, int startX, int startY) : Actor(sw, IID_WALL, startX, startY, none) {}
    virtual ~Wall() {}
    
    virtual void doSomething() { return; }
    virtual bool allowsAgentColocation() { return false; }
    virtual bool stopsPeas() { return true; }
};

class Robot : public Agent {
public:
    Robot(StudentWorld* sw, int imageID, int startX, int startY, int dir) : Agent(sw, imageID, startX, startY, dir), m_ticks(0) {}
    virtual ~Robot() {}
    
    virtual bool checkPosForMarble(int dir) { return false; }
    //virtual bool canPush(int dir) { return false; }
    virtual bool isMean() { return true; }
    virtual bool needsClearShot() const { return true; }
    //virtual bool willShoot();
    virtual void changeDirection(int dir);
    virtual void pushPea(int dir);

    virtual bool shouldIShoot();
    bool timeToAct();
    int getTicks() { return m_ticks; }
    void addTicks() { m_ticks++; }
    void resetTicks() { m_ticks = 0; }
    //bool foundAttackableAvatar(int dir);
    //bool checkHorizontal(int dir);
    //bool checkVertical(int dir, int posX, int posY);
    //bool canIAttackAt(int posX, int posY);
private:
    int m_ticks;
};

class RageBot : public Robot {
public:
    RageBot(StudentWorld* sw, int startX, int startY, int dir) : Robot(sw, IID_RAGEBOT, startX, startY, dir) {
        setHealth(10);
    }
    virtual ~RageBot() {}
    
    virtual void loseHealth();
    virtual void doSomething();
    //virtual void setDead() { Actor::setDead(); }
};

class ThiefBot : public Robot {
public:
    ThiefBot(StudentWorld* sw, int imageID, int startX, int startY) : Robot(sw, imageID, startX, startY, right), m_distanceBeforeTurning(6), m_distanceTicker(0) {}
    virtual ~ThiefBot() {}
    
    //if this object colocates another, can they steal it? (thiefbot indi)
    virtual bool canSteal() { return true; }

    
    
    
    virtual bool countsForFactoryCount() { return true; }
    virtual void doSomething();
    virtual void loseHealth();

    int getDistanceBeforeTurning() { return m_distanceBeforeTurning; }
    void setNewDistanceBeforeTurning() { m_distanceBeforeTurning = (rand()&6)+1; }
    
    int getDistanceTicker() { return m_distanceTicker; }
    void incDistanceTicker() { m_distanceTicker++; }
    void resetDistanceTicker() { m_distanceTicker = 0; }
private:
    int m_distanceBeforeTurning;
    int m_distanceTicker;
};

class MeanThiefBot : public ThiefBot {
public:
    MeanThiefBot(StudentWorld* sw, int startX, int startY) : ThiefBot(sw, IID_MEAN_THIEFBOT, startX, startY) {
        setHealth(8);
    }
    virtual ~MeanThiefBot() {}
};

class RegularThiefBot : public ThiefBot {
public:
    RegularThiefBot(StudentWorld* sw, int startX, int startY) : ThiefBot(sw, IID_THIEFBOT, startX, startY) {
        setHealth(5);
    }
    virtual ~RegularThiefBot() {}
    
    virtual bool shouldIShoot() { return false; }
    virtual bool isMean() { return false; }
};

class ThiefBotFactory : public Actor {
public:
    ThiefBotFactory(StudentWorld*sw, int startX, int startY, int meanOrReg) : Actor(sw, IID_ROBOT_FACTORY, startX, startY), m_typeOfRobot(meanOrReg) {}
    virtual void doSomething();
    virtual bool allowsAgentColocation() { return false; }
    virtual bool stopsPeas() { return true; }
    virtual ~ThiefBotFactory() {}
private:
    int m_typeOfRobot; //3 is mean and 2 is regular
};

class PickupableItem : public Actor {
public:
    PickupableItem(StudentWorld* sw, int imageID, int startX, int startY) : Actor(sw, imageID, startX, startY) {}
    virtual ~PickupableItem() {}
    
    virtual bool allowsAgentColocation() { return true; };
    virtual bool blocksBotRange() { return false; }
};

class Crystal : public PickupableItem {
public:
    Crystal(StudentWorld* sw, int startX, int startY) : PickupableItem(sw, IID_CRYSTAL, startX, startY) {}
    virtual ~Crystal() {}
    
    virtual void doSomething();
};

class Goodie : public PickupableItem {
public:
    Goodie(StudentWorld* sw, int imageID, int startX, int startY) : PickupableItem(sw, imageID, startX, startY) {}
    virtual ~Goodie() {}

    virtual void doSomething();
    virtual bool isStealable() { return true; }

    virtual void giveGoodie() = 0;
    virtual void givePoints() = 0;
};

class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(StudentWorld* sw, int startX, int startY) : Goodie(sw, IID_EXTRA_LIFE, startX, startY) {}
    virtual ~ExtraLifeGoodie() {}

    virtual void giveGoodie();
    virtual void givePoints();
    virtual bool givesLife() { return true; }
};

class RestoreHealthGoodie : public Goodie {
public:
    RestoreHealthGoodie(StudentWorld* sw, int startX, int startY) : Goodie(sw, IID_RESTORE_HEALTH, startX, startY) {}
    virtual ~RestoreHealthGoodie() {}

    virtual void giveGoodie();
    virtual void givePoints();
    virtual bool givesHealth() { return true; }
};

class AmmoGoodie : public Goodie {
public:
    AmmoGoodie(StudentWorld* sw, int startX, int startY) : Goodie(sw, IID_AMMO, startX, startY) {}
    virtual ~AmmoGoodie() {}

    virtual void giveGoodie();
    virtual void givePoints();
    virtual bool givesAmmo() { return true; }
};

class Exit : public Actor {
public:
    Exit(StudentWorld* sw, int startX, int startY) : Actor(sw, IID_EXIT, startX, startY), m_currentVisibility(false) {}
    virtual ~Exit() {}

    virtual bool allowsAgentColocation() { return true; }
    virtual void doSomething();
    
    bool getCurrentVisibility() { return m_currentVisibility; }
    void setCurrentVisibility(bool m) { m_currentVisibility = m; }
private:
    bool m_currentVisibility;
};

//class Pea : public Actor {
//public:
//    Pea(StudentWorld* sw, int startX, int startY, int direction) : Actor(sw, IID_PEA, startX, startY, direction) {}
//
//    virtual void doSomething();
//    virtual bool allowsAgentColocation() { return false; }
//private:
//    bool m_currentVisibility;
//};

class Marble : public Actor {
public:
    Marble(StudentWorld* sw, int startX, int startY) : Actor(sw, IID_MARBLE, startX, startY) {
        setHealth(10);
    }
    virtual ~Marble() {}
    
    //if a pea colocates, will it be damaged?
    virtual bool isDamageable() { return true; }
    virtual bool stopsPeas() { return true; }

    
    
    
    virtual void doSomething();
    virtual bool allowsAgentColocation() { return false; }
    virtual bool isSwallowable() { return true; }
    virtual bool canDie() { return true; }
};

class Pit : public Actor {
public:
    Pit(StudentWorld* sw, int startX, int startY) : Actor(sw, IID_PIT, startX, startY) {}
    virtual ~Pit() {}
    
    virtual void doSomething() { return; }
    virtual bool allowsMarble() { return true; }
    virtual bool allowsAgentColocation() { return false; }
    virtual bool allowsMarbleColocation() { return true; }
    virtual bool blocksBotRange() { return false; }
};

#endif // ACTOR_H_
