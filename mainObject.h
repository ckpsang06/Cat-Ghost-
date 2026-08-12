#pragma once
#include "Entity.h"

class Cat : public Entity {
private:
    int Health;
public:
    Cat();
    int getHealth();
    void setHealth(int p_h);
    void update();
};
