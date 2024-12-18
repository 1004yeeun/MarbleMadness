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
    virtual int init(); //initializes the level file and sets up the level
    virtual int move(); //called for every tick, making every object do something
    virtual void cleanUp(); //clears all actors and the m_avatar pointer
    ~StudentWorld() { cleanUp(); } //destructor using "helper function"
    
    //bonus points
    void setBonus() { m_bonus = 1000; } //starts at 1000
    void decBonus() { if (m_bonus>0) m_bonus--; }
    //decreases the m_bonus by a point per tick
    unsigned int getBonus() { return m_bonus; }
    //returns the value of m_bonus
    
    //level completion
    bool isLevelCompleted () { return m_levelCompleted; }
    void setLevelCompletion(bool hm) { m_levelCompleted = hm; }
    
    //finding crystals
    void foundACrystal() { m_crystal--; }
    //decrease the number of crystals to find when finding one
    bool foundAllCrystals() { return m_crystal==0; }
    //if you found all of the crystals, return true
    void setCrystals() { m_crystal = 0; }
    //set the number of crystals to find to zero
    void addCrystal() { m_crystal++; }
    //add a crystal to find each time one is added to the level
    
    //adding peas
    void addPea(int x, int y, int dir);
    //add the pea to the vector, bring it to life!
    bool killsPea(int x, int y);
    bool peaDoesDamage(int x, int y);
    
    //if a thiefbot that stole dies, return a new goodieType into its x,y
    void returnAGoodie(int x, int y, int goodieType);

    
    int surroundingCount(int x, int y);
    bool isThiefBotAt(int x, int y);
    void addRegularThiefBot(int x, int y);
    void addMeanThiefBot(int x, int y);
    Actor* getGoodie(int x, int y);
    
    void giveAmmo();
    void giveHealth();
    
    bool foundTargetInSight(int x, int y, int dir);
    
    bool isValidPos(int x, int y);
    bool isValidPos(int x, int y, Actor* actor);

    void getAvatarPos(int& posX, int& posY);
    bool playerIsStandingOn(Actor* actor);
    bool overlaps(Actor* actor1, Actor* actor2);
    bool equalPos(int x1, int y1, int x2, int y2);
    bool isValidBotTarget(int x, int y, Actor* actor);
    bool isValidBotTarget(int x, int y);

    bool isMarble(int x, int y);
    bool moveMarble(int x, int y, int dir);
    Actor* getMarble(int x, int y);
    bool isValidMarblePos(int x, int y);
    bool isGoodie(int x, int y, int& goodieType, int& goodieX, int& goodieY);
        
    bool isPit(int x, int y);
    void killPit(int x, int y);
    Actor* getPit(int x, int y);
    
    int getPlayersX() { return m_avatar->getX(); }
    int getPlayersY() { return m_avatar->getY(); }
    Actor* getActorAt(int x, int y);
    
    bool getSoundTemp() { return m_soundTemp; }
    void fillSoundTemp() { m_soundTemp = true; }
    void setSoundTemp() { m_soundTemp = false; } //initializes bool m_soundTemp
    //used to control the sound being played when a crystal is obtained at the same time the exit reveals itself
    
private:
    Avatar* m_avatar;
    vector<Actor*> actors;
    bool m_levelCompleted;
    int m_crystal;
    unsigned int m_bonus;
    bool m_soundTemp;
};

#endif // STUDENTWORLD_H_
