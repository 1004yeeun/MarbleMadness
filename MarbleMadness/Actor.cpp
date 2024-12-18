#include "Actor.h"
#include "StudentWorld.h"
//
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
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

bool Agent::move(int dir) {
    if (canMove(dir)) {
        int tempX, tempY;
        newPos(dir, tempX, tempY);
        moveTo(tempX,tempY);
        return true;
    }
    return false;
}

void Avatar::doSomething() {
    int ch;
    if (getWorld()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_SPACE:
                if (m_peas>0) {
                    decPea();
                }
                break;
            case KEY_PRESS_ESCAPE:
                setDead();
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
}
