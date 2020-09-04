#ifndef HUD_H
#define HUD_H

#include "global.h"
#include "player.h"

extern SDL_Texture* hud_texture;
extern SDL_Texture* numbers_texture;
extern SDL_Texture* velocimeter_texture;

void hud_load();

void hud_drawnumber(int x, int y, double number);

void hud_drawhud(player* p);

void hud_destroy();

#endif
