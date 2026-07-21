#pragma once

#include<bits/stdc++.h>
#include<SDL.h>

using namespace std;

const int SCREEN_WIDTH = 1224;
const int SCREEN_HEIGHT = 800;

const int FPS = 60;
const int frameDelay = 1000 / FPS;

const int totalCatFrames = 20;
const int totalGhostFrames = 8;
const int totalAttackFrames = 4;

const SDL_Color whiteColor = {255, 255, 255, 255};
const SDL_Color redColor = {255, 0, 0, 50};
const SDL_Color greenColor = {0, 255, 0, 50};
const SDL_Color blueColor = {0, 0, 255, 50};
const SDL_Color yellowColor = {255, 255, 0, 255};
const SDL_Color pinkColor = {255, 105, 180, 255};
const SDL_Color blackColor = {0, 0, 0, 255};
const SDL_Color transparent = {0, 0, 0, 0};

const int spawnGhostDelay = 300;
const int CatHealth = 5;

