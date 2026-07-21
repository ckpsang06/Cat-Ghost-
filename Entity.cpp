#include "Entity.h"

Entity::Entity() {
    x = y = 0;
    width = height = 0;
    currentFrame = 0;
    texture = NULL;
    frameStart = 0;
    flip = SDL_FLIP_NONE;
}

void Entity::init(SDL_Texture *p_tex, int p_w, int p_h, int numFrames, int frame_delay) {
    texture = p_tex;
    width = p_w;
    height = p_h;
    frameDelay = frame_delay;
    int w_tex = 0, h_tex = 0;
    SDL_QueryTexture(p_tex, NULL, NULL, &w_tex, &h_tex);
    int w_frame = w_tex / numFrames, h_frame = h_tex;
    for (int i = 0; i < numFrames; i++) {
        frames.push_back({i * w_frame, 0, w_frame, h_frame});
    }
}

void Entity::setX(double p_x) { x = p_x; }
void Entity::setY(double p_y) { y = p_y; }
void Entity::setFrameStart(int p_fs) { frameStart = p_fs; }
void Entity::setFrameDelay(int p_fd) { frameDelay = p_fd; }
void Entity::setFlip(SDL_RendererFlip p_flip) { flip = p_flip; }

double Entity::getX() { return x; }
double Entity::getY() { return y; }
int Entity::getWidth() { return width; }
int Entity::getHeight() { return height; }
SDL_Texture *Entity::getTexture() { return texture; }
SDL_RendererFlip Entity::getFlip() { return flip; }

void Entity::nextFrame() {
    currentFrame++;
    if (currentFrame >= (int)frames.size()) currentFrame = 0;
}

int Entity::getCur() { return currentFrame; }
void Entity::setCurrentFrame(int cur) { currentFrame = cur; }
int Entity::getNumFrames() { return (int)frames.size(); }

SDL_Rect Entity::getCurrentFrame() { return frames[currentFrame]; }
SDL_Rect Entity::getFrame(int id) { return frames[id]; }
