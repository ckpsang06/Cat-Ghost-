#include "mainObject.h"

Cat::Cat() {
    Health = 0;
}

void Cat::setHealth(int p_h) {
    Health = p_h;
}

int Cat::getHealth() {
    return Health;
}

void Cat::update() {
    if (SDL_GetTicks() - frameStart >= frameDelay) {
        nextFrame();
        frameStart = SDL_GetTicks();
    }
}
