#include "Ghost.h"
#include <cassert>
#include <random>
#include <chrono>

using namespace std;

Ghost::Ghost() {
    ghostSpeed = 3;
    _isDead = 0;
    _isAttacking = 0;
}


void Ghost::update() {
    if (SDL_GetTicks() - frameStart >= frameDelay) {
        nextFrame();
        frameStart = SDL_GetTicks();
        if (_isAttacking && currentFrame == 0) {
            _isDead = true;
        }
    }
}

void Ghost::setDead() {
    _isDead = 1;
}

bool Ghost::isDead() {
    return _isDead;
}

bool Ghost::isAttacking() { return _isAttacking; }
void Ghost::setAttacking() { _isAttacking = true; }

mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());
long long RandNum(long long l, long long r) {
    assert(l <= r);
    return uniform_int_distribution<long long>(l, r)(rd);
}

double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void Ghost::GhostMoving(double targetX, double targetY) {
    double dist = distance(x, y, targetX, targetY);
    if (dist == 0) return;
    double dirX = 1.0 * (targetX - x) / dist;
    double dirY = 1.0 * (targetY - y) / dist;
    x += dirX * ghostSpeed;
    y += dirY * ghostSpeed;
    //<<x<<" "<<y<<"\n";
}
