#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Avatar::loseHealth() {
    Actor::loseHealth();
    if (getHealth()<=0) {
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
}//$

void Avatar::doSomething() {
    if (!isAlive()) { return; }
    int ch;
    if (getWorld()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_ESCAPE:
                getWorld()->playSound(SOUND_PLAYER_DIE);
                setDead();
                break;
            case KEY_PRESS_SPACE:
                if (getPeas()>0) {
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    decPea();
                    pushPea(getDirection());
                }
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                move(left);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                move(right);
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                move(up);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                move(down);
                break;
        }
    }
    if (getHealth()<=0) {
        setDead();
        return;
    }
}

void Avatar::pushPea(int dir) {
    switch (dir) {
        case right:
            getWorld()->addPea(getX(),getY(),dir);
            break;
        case left:
            getWorld()->addPea(getX(),getY(),dir);
            break;
        case up:
            getWorld()->addPea(getX(),getY(),dir);
            break;
        case down:
            getWorld()->addPea(getX(),getY(),dir);
            break;
    }
}//$

void Robot::pushPea(int dir) {
    switch (dir) {
        case right:
            getWorld()->addPea(getX(),getY(),dir);
            break;
        case left:
            getWorld()->addPea(getX(),getY(),dir);
            break;
        case up:
            getWorld()->addPea(getX(),getY(),dir);
            break;
        case down:
            getWorld()->addPea(getX(),getY(),dir);
            break;
    }
}//$

void Pea::doSomething() {
    if (!isAlive()) { return; }
    int nextX = getX();
    int nextY = getY();
    switch (getDirection()) {
        case left:
            nextX--;
            break;
        case right:
            nextX++;
            break;
        case up:
            nextY++;
            break;
        case down:
            nextY--;
            break;
    }
    //if (willItDie()) {
    //    setDead();
    //    return;
    //}
    //if (getWorld()->peaDoesDamage(nextX,nextY)) {
    //    moveTo(nextX,nextY);
    //    //goingToDieSoon();
    //    setDead();
    //    return;
    //}
    if (willItDie()) {
        setDead();
        return;
    }
    if (getWorld()->killsPea(nextX,nextY)) {
        if (getWorld()->killsPea(getX(),getY())) {
            goingToDieSoon();
            return;
        }
        setDead();
        return;
    }
    else {
        moveTo(nextX,nextY);
    }
    //if (getWorld()->peaDoesDamage(getX(),getY())) {
    //    setDead();
    //    return;
    //}
    //if (getWorld()->interactsPea(getX(),getY())) {
    //    setDead();
    //    return;
    //}
    //if (getWorld()->killsPea(getX(),getY())) {
    //    setDead();
    //    return;
    //}
    //else {
     //   switch (getDirection()) {
     //       case left:
     //           moveTo(getX()-1,getY());
     //           return;
     //       case right:
     //           moveTo(getX()+1,getY());
     //           return;
     //       case up:
     //           moveTo(getX(),getY()+1);
     ////           return;
     //       case down:
    //            moveTo(getX(),getY()-1);
    ////            return;
     //   }
    //}
    return;
}







void ThiefBot::loseHealth() {
    Actor::loseHealth();
    if (getHealth()<=0) {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(10);
    }
}

void RageBot::loseHealth() {
    Actor::loseHealth();
    if (getHealth()<=0) {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(100);
    }
}

void RageBot::doSomething() {
    if (!isAlive()) { return; }
    if (getHealth()<=0) {
        setDead();
        return;
    }
    
    if (timeToAct()) {
        if (shouldIShoot()) {
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            pushPea(getDirection());
            return;
        }
        if (!move(getDirection())) {
            changeDirection(getDirection());
        }
    }
}

bool Robot::shouldIShoot() {
    switch (getDirection()) {
        case left:
            if (getWorld()->foundTargetInSight(getX(),getY(),left)) {
                return true;
            }
            break;
        case right:
            if (getWorld()->foundTargetInSight(getX(),getY(),right)) {
                return true;
            }
            break;
        case up:
            if (getWorld()->foundTargetInSight(getX(),getY(),up)) {
                return true;
            }
            break;
        case down:
            if (getWorld()->foundTargetInSight(getX(),getY(),down)) {
                return true;
            }
            break;
    }
    return false;
}

void ThiefBot::doSomething() {
    if (!isAlive()) {
        return;
    }
    if (timeToAct()) {
        if (shouldIShoot()) {
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            pushPea(getDirection());
            return;
        }
        int goodieType, goodieX, goodieY;
        if (getWorld()->isGoodie(getX(),getY(),goodieType,goodieX,goodieY)) {
            setGoodieType(goodieType);
            if ((!getStealingStatus()) && ((rand()&10)==0)) {
                getWorld()->playSound(SOUND_ROBOT_MUNCH);
                stoleAGoodie();
                getWorld()->getGoodie(goodieX,goodieY)->setStolen();
                return;
            }
        }
        
        if (getDistanceTicker()!=getDistanceBeforeTurning()) {
            if (!move(getDirection())) {
                changeDirection(rand()%4 + 1);
                setNewDistanceBeforeTurning();
                resetDistanceTicker();
                return;
            }
            incDistanceTicker();
            return;
        }
        resetDistanceTicker();
    }
    if (getHealth()<=0) {
        setDead();
    }
    return;
}

bool Robot::timeToAct() {
    if ( getTicks() >= (28 - getWorld()->getLevel()) / 4 ) {
        resetTicks();
        return true;
    }
    addTicks();
    return false;
}

void Robot::changeDirection(int dir) {
    if (dir==180 || dir==1) {
        setDirection(right);
    }
    else if (dir==0 || dir==2) {
        setDirection(left);
    }
    else if (dir==90 || dir==3) {
        setDirection(down);
    }
    else if (dir==270 || dir==4) {
        setDirection(up);
    }
}

void ThiefBotFactory::doSomething() {
    if (!isAlive()) { return; }
    if (getWorld()->surroundingCount(getX(),getY())<3 && !(getWorld()->isThiefBotAt(getX(),getY()))) {
        if (rand()%50 == 0) {
            if (m_typeOfRobot==2) { //regular thiefbot
                getWorld()->addRegularThiefBot(getX(),getY());
            }
            if (m_typeOfRobot==3) { //mean thiefbot
                getWorld()->addMeanThiefBot(getX(),getY());
            }
        }
    }
}

void Marble::doSomething() {
    if (!isAlive()) { return; }
    if (getHealth()<=0) {
        setDead();
        return;
    }
    if (getWorld()->isPit(getX(),getY())) {
        getWorld()->killPit(getX(),getY());
        setDead();
        return;
    }
}

void Exit::doSomething() {
    if (!isAlive()) { return; }
    setVisible(false);
    if ( getWorld()->foundAllCrystals() ) {
        getWorld()->playSound(SOUND_REVEAL_EXIT);
        setCurrentVisibility(true);
        setVisible(true);
    }
    if (getWorld()->playerIsStandingOn(this) && getCurrentVisibility()) {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->setLevelCompletion(true);
        setDead();
        return;
    }
}

void Crystal::doSomething() {
    if (!isAlive()) { return; }
    if (getWorld()->playerIsStandingOn(this)) {
        getWorld()->increaseScore(50);
        getWorld()->foundACrystal();
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        return;
    }
}

void AmmoGoodie::giveGoodie() {
    getWorld()->giveAmmo();
}
void AmmoGoodie::givePoints() {
    getWorld()->increaseScore(100);
}

void RestoreHealthGoodie::giveGoodie() {
    getWorld()->giveHealth();
}
void RestoreHealthGoodie::givePoints() {
    getWorld()->increaseScore(500);
}

void ExtraLifeGoodie::giveGoodie() {
    getWorld()->incLives();
}
void ExtraLifeGoodie::givePoints() {
    getWorld()->increaseScore(1000);
}

void Goodie::doSomething() {
    if (!isAlive()) { return; }
    if (getWorld()->playerIsStandingOn(this) && !isItStolen()) {
        givePoints();
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        giveGoodie();
        return;
    }
    if (isItStolen()) {
        setDead();
        return;
    }
}

void Agent::newPos(int dir, int& newX, int& newY) {
    int tempX = getX();
    int tempY = getY();
    switch (dir) {
        case left:
            tempX -= 1;
            break;
        case right:
            tempX +=1;
            break;
        case up:
            tempY += 1;
            break;
        case down:
            tempY -=1;
            break;
    }
    newX = tempX;
    newY = tempY;
}

bool Agent::canMove(int dir) {
    int tempX, tempY;
    newPos(dir, tempX, tempY);
    return getWorld()->isValidPos(tempX,tempY,this);
}

//bool Avatar::canPush(int dir) {
//    switch (dir) {
//        case left:
//            if (getWorld()->isMarble(getX()-1,getY())) {
//                if (getWorld()->isValidPos(getX()-2,getY())
//                    && !getWorld()->blocksMarble(getX()-2,getY())) {
//                    moveTo(getX()-1,getY());
//                }
//                return true;
//            }
//            break;
//        case right:
//            if (getWorld()->isMarble(getX()+1,getY())) {
//                if (getWorld()->isValidPos(getX()+2,getY())
//                    && !getWorld()->blocksMarble(getX()+2,getY())) {
//                    moveTo(getX()+1,getY());
//                }
//                return true;
//            }
//            break;
//        case up:
//            if (getWorld()->isMarble(getX(),getY()+1)) {
//                if (getWorld()->isValidPos(getX(),getY()+2)
//                    && !getWorld()->blocksMarble(getX(),getY()+2)) {
//                    moveTo(getX(),getY()+1);
//                }
//                return true;
//            }
//            break;
//        case down:
//            if (getWorld()->isMarble(getX(),getY()-1)) {
//                if (getWorld()->isValidPos(getX(),getY()-2)
//                    && !getWorld()->blocksMarble(getX(),getY()-2)) {
//                    moveTo(getX(),getY()-1);
//                }
//                return true;
//            }
//            break;
//    }
//    return false;
//}

bool Avatar::checkPosForMarble(int dir) {
    switch (dir) {
        case left:
            if (getWorld()->isMarble(getX()-1,getY())) {
                return true;
            }
            break;
        case right:
            if (getWorld()->isMarble(getX()+1,getY())) {
                return true;
            }
            break;
        case up:
            if (getWorld()->isMarble(getX(),getY()+1)) {
                return true;
            }
            break;
        case down:
            if (getWorld()->isMarble(getX(),getY()-1)) {
                return true;
            }
            break;
    }
    return false;
}

bool Agent::move(int dir) {
    if (canMove(dir)) {
        int tempX, tempY;
        newPos(dir, tempX, tempY);
        moveTo(tempX,tempY);
        return true;
    }
    if (checkPosForMarble(dir)) {
        switch (dir) {
            case left:
                if (getWorld()->moveMarble(getX()-1,getY(),dir)) {
                    moveTo(getX()-1,getY());
                }
                break;
            case right:
                if (getWorld()->moveMarble(getX()+1,getY(),dir)) {
                    moveTo(getX()+1,getY());
                }
                break;
            case up:
                if (getWorld()->moveMarble(getX(),getY()+1,dir)) {
                    moveTo(getX(),getY()+1);
                }
                break;
            case down:
                if (getWorld()->moveMarble(getX(),getY()-1,dir)) {
                    moveTo(getX(),getY()-1);
                }
                break;
        }
        return true;
    }
    return false;
}
