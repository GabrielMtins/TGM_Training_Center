#ifndef TITLE_H
#define TITLE_H

#include "global.h"

#define NO_SCREEN 0
#define TITLE_SCREEN 1
#define LEVEL_SELECT_SCREEN 2
#define STORY_SCREEN 3
#define HTP_SCREEN 4
#define GAMEOVER_SCREEN 5
#define YOUWIN_SCREEN 6

SDL_Texture* texture_load(char* filename);

typedef struct{
    int current_state;
    SDL_Texture* screen_levelselect;
    SDL_Texture* screen_title;
    SDL_Texture* screen_howtoplay;
    SDL_Texture* screen_gameover;
    SDL_Texture* screen_win;
    SDL_Texture* screen_story;
} menu;

int menu_isMouseInsideRect(SDL_Rect r);

menu* menu_create();

void menu_update(menu* t);

void menu_show(menu* t);

void menu_destroy(menu* t);

#endif
