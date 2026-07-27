#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

class Entity {
protected:
    double x, y, width, height;
    SDL_Texture *texture;
    std::vector<SDL_Rect> frames;
    int currentFrame;
    int frameStart, frameDelay;
    SDL_RendererFlip flip;

public:
    Entity();
    void init(SDL_Texture *p_tex, int p_w, int p_h, int numFrames, int frame_delay);
    void setX(double p_x);
    void setY(double p_y);
    void setFrameStart(int p_fs);
    void setFrameDelay(int p_fd);
    void setFlip(SDL_RendererFlip p_flip);
    
    double getX();
    double getY();
    int getWidth();
    int getHeight();
    SDL_Texture *getTexture();
    SDL_RendererFlip getFlip();
    
    void nextFrame();
    int getCur();
    void setCurrentFrame(int cur);
    int getNumFrames();
    
    SDL_Rect getFrame(int id);
    SDL_Rect getCurrentFrame();
};
