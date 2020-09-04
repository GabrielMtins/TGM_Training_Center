#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_VELOCITY 10
#define PLAYER_ACCELERATION 10

#include "mode7.h"
#include "sprite.h"

extern double timeTakenToFinish;
extern double high_score[4];
extern int hs_currentlevel;

typedef struct{
    double x, y;
    double vx, vy;
    double accx, accy;
    double direction;
    double fuel;
    uint8_t hasFinished;
    mode7_camera* player_camera;
    SDL_Texture* sky_texture;
    int width_collision, height_collision;
    uint8_t* collision_map;
    sprite* listOfSprites;
    int numOfSprites;
} player;

extern player* mainPlayer;

void player_loadHighScores(char* filename);

void player_saveHighScores(char* filename);

player* player_create(char* map_filename);

void player_destroyCollisionMap(player* p);

void player_loadCollisionMap(player* p, char* collision_filename);

void player_loadSky(player* p, char* sky_filename);

void player_renderSprites(player* p);

void player_input(player* p);

void player_update(player* p);

void player_render(player* p);

void player_destroy(player* p);

#endif
