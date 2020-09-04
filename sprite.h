#ifndef SPRITE_H
#define SPRITE_H

#include "global.h"

typedef struct{
    SDL_Texture* spr_texture;
    double width, height;
} sprite_profile;

typedef struct{
    double x, y;
    sprite_profile* spr_profile;
} sprite;

extern sprite_profile* allSpriteProfiles;

void sprite_loadAllProfiles(char* filename);

void sprite_render(sprite* s); // camera must be in 0,0

void sprite_destroyProfiles();

#endif
