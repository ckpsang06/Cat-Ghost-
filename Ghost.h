#pragma once

#include "Entity.h"

long long RandNum(long long l, long long r);
double distance(int x1, int y1, int x2, int y2);

class Ghost : public Entity {
private:
    int ghostSpeed;
    bool _isDead;
public:
    Ghost();
    void update();
    void GhostDie();
    void GhostAttack();
    void GhostMoving(double targetX, double targetY);;
    bool isDead();
    void setDead();

};
