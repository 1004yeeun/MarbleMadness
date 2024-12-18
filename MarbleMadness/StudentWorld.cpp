#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_avatar = nullptr;
    actors.clear();
}

int StudentWorld::init()
{
    setLevelCompletion(false);
    
    string curLevel = "level0" + to_string(getLevel()) + ".txt";
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found) {
        cerr << "Cannot find " + curLevel + " data file" << endl; //can we write to cerr??????????????????????ASKED
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << "Level file is not correctly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success) {
        cerr << "Level file was successfully loaded" << endl;
        for (int x=0; x<VIEW_WIDTH; x++) {
            for (int y=0; y<VIEW_HEIGHT;y++) {
                switch( lev.getContentsOf(x,y) ) {
                    case Level::empty:
                        cerr << to_string(x) + "," + to_string(y) + " should be empty in the maze" << endl;
                        break;
                    case Level::exit:
                        cerr << to_string(x) + "," + to_string(y) + " should be an exit in the maze" << endl;
                        //actors.push_back(new Exit(this,x,y));
                        break;
                    case Level::player:
                        cerr << to_string(x) + "," + to_string(y) + " should be a player in the maze" << endl;
                        m_avatar = new Avatar(this,x,y);
                        break;
                    case Level::horiz_ragebot:
                        cerr << to_string(x) + "," + to_string(y) + " should be a horiz_ragebot in the maze" << endl;
                        //actors.push_back(new RageBot(this,x,y,0));
                        break;
                    case Level::vert_ragebot:
                        cerr << to_string(x) + "," + to_string(y) + " should be a vert_ragebot in the maze" << endl;
                        //actors.push_back(new RageBot(this,x,y,270));
                        break;
                    case Level::thiefbot_factory:
                        cerr << to_string(x) + "," + to_string(y) + " should be a thiefbot_factory in the maze" << endl;
                        //actors.push_back(new Robot_Factory(this,x,y));
                        break;
                    case Level::mean_thiefbot_factory:
                        cerr << to_string(x) + "," + to_string(y) + " should be a mean_thiefbot_factory in the maze" << endl;
                        //actors.push_back(new Mean_ThiefBot(this,x,y));
                        break;
                    case Level::wall:
                        cerr << to_string(x) + "," + to_string(y) + " should be a wall in the maze" << endl;
                        addWall(x,y);
                        //actors.push_back(new Wall(this,x,y));
                        break;
                    case Level::marble:
                        cerr << to_string(x) + "," + to_string(y) + " should be a marble in the maze" << endl;
                        //actors.push_back(new Marble(this,x,y));
                        break;
                    case Level::pit:
                        cerr << to_string(x) + "," + to_string(y) + " should be a pit in the maze" << endl;
                        //actors.push_back(new Pit(this,x,y));
                        break;
                    case Level::crystal:
                        cerr << to_string(x) + "," + to_string(y) + " should be a crystal in the maze" << endl;
                        //actors.push_back(new Crystal(this,x,y));
                        break;
                    case Level::restore_health:
                        cerr << to_string(x) + "," + to_string(y) + " should be a restore_health in the maze" << endl;
                        //actors.push_back(new RestoreHealthGoodie(this,x,y));
                        break;
                    case Level::extra_life:
                        cerr << to_string(x) + "," + to_string(y) + " should be a extra_life in the maze" << endl;
                        //actors.push_back(new ExtraLifeGoodie(this,x,y));
                        break;
                    case Level::ammo:
                        cerr << to_string(x) + "," + to_string(y) + " should be ammo in the maze" << endl;
                        //actors.push_back(new AmmoGoodie(this,x,y));
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_avatar->doSomething();
    for (auto it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();
            
            if (!m_avatar->isAlive()) {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            
            if (m_levelCompleted) {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    
    for (auto it = actors.begin(); it != actors.end(); it++) {
        if (!(*it)->isAlive()) {
            delete *it;
            actors.erase(it);
            it = actors.begin();
        }
    }
    
    setGameStatText("Game will end when you type q");
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (vector<Actor*>::iterator it=actors.begin(); it!=actors.end(); it++) {
        delete *it;
    }
    actors.clear();
    delete m_avatar;
    m_avatar = nullptr;
}

bool StudentWorld::addWall(int x, int y) {
    if (isValidPos(x,y)) {
        actors.push_back(new Wall(this,x,y));
        return true;
    }
    return false;
}

bool StudentWorld::isValidPos(int x, int y) {
    return isValidPos(x,y,nullptr);
}

bool StudentWorld::isValidPos(int x, int y, Actor* actor) {
    for (auto a : actors) {
        if (a != actor) {
            if (a->isAlive() && !(a->allowsAgentColocation())) {
                if (a->getX()==x && a->getY()==y) {
                    return false;
                }
            }
        }
    }
    return true;
}
