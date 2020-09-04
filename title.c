#include "title.h"
#include "load.h"
#include "hud.h"

#include <stdio.h>
#include <stdlib.h>

SDL_Texture* texture_load(char* filename){
    SDL_Surface* surface = IMG_Load(filename);
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return t;
}

int menu_isMouseInsideRect(SDL_Rect r){
    int x_mouse_pos, y_mouse_pos;
    if(SDL_GetMouseState(&x_mouse_pos, &y_mouse_pos)&SDL_BUTTON(SDL_BUTTON_LEFT)){
        if(x_mouse_pos > r.x && y_mouse_pos > r.y && x_mouse_pos < r.x+r.w && y_mouse_pos < r.y + r.h) return 1;
        else return 0;
    }
    else return 0;
}

menu* menu_create(){
    menu* m = malloc(sizeof(menu));
    m->screen_levelselect = texture_load("textures/menu/level_select.png");
    m->screen_title = texture_load("textures/menu/titlescreen.png");
    m->screen_howtoplay = texture_load("textures/menu/how_to_play.png");
    m->screen_gameover = texture_load("textures/menu/gameover.png");
    m->screen_win = texture_load("textures/menu/win.png");
    m->screen_story = texture_load("textures/menu/story.png");
    m->current_state = TITLE_SCREEN;
    return m;
}

void menu_update(menu* t){
    if(t->current_state == TITLE_SCREEN){
        SDL_Rect play_button = {7.03*TILE_SIZE, 4.95*TILE_SIZE, 1.95*TILE_SIZE, 0.85*TILE_SIZE};
        if(menu_isMouseInsideRect(play_button)){
            t->current_state = LEVEL_SELECT_SCREEN;
        }
        SDL_Rect htp_button = {5.9*TILE_SIZE, 5.98*TILE_SIZE, 4.2*TILE_SIZE, 0.80*TILE_SIZE};
        if(menu_isMouseInsideRect(htp_button)) t->current_state = HTP_SCREEN;
        SDL_Rect story_button = {6.76*TILE_SIZE, 6.975*TILE_SIZE, 2.55*TILE_SIZE, 0.825*TILE_SIZE};
        if(menu_isMouseInsideRect(story_button)) t->current_state = STORY_SCREEN;
    }
    if(t->current_state == LEVEL_SELECT_SCREEN){
        if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_RIGHT)) t->current_state = TITLE_SCREEN;
        SDL_Rect button1 = {1.03*TILE_SIZE, 2.2875*TILE_SIZE, 2.55*TILE_SIZE, 2.55*TILE_SIZE};
        if(menu_isMouseInsideRect(button1)){
            t->current_state = NO_SCREEN;
            load_fromMap("levels/road_01.map");
            hs_currentlevel = 0;
        }
        SDL_Rect button2 = {4.78*TILE_SIZE, 2.2875*TILE_SIZE, 2.55*TILE_SIZE, 2.55*TILE_SIZE};
        if(menu_isMouseInsideRect(button2)){
            t->current_state = NO_SCREEN;
            load_fromMap("levels/road_02.map");
            hs_currentlevel = 1;
        }
        SDL_Rect button3 = {8.5*TILE_SIZE, 2.2875*TILE_SIZE, 2.55*TILE_SIZE, 2.55*TILE_SIZE};
        if(menu_isMouseInsideRect(button3)){
            t->current_state = NO_SCREEN;
            load_fromMap("levels/road_03.map");
            hs_currentlevel = 2;
        }
        SDL_Rect button4 = {12.225*TILE_SIZE, 2.2875*TILE_SIZE, 2.55*TILE_SIZE, 2.55*TILE_SIZE};
        if(menu_isMouseInsideRect(button4)){
            t->current_state = NO_SCREEN;
            load_fromMap("levels/road_04.map");
            hs_currentlevel = 3;
        }
    }
    if(t->current_state == HTP_SCREEN){
        if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_RIGHT)){
            t->current_state = TITLE_SCREEN;
        }
    }
    if(t->current_state == STORY_SCREEN){
        if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_RIGHT)){
            t->current_state = TITLE_SCREEN;
        }
    }
    if(t->current_state == GAMEOVER_SCREEN){
        if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_LEFT)){
            t->current_state = TITLE_SCREEN;
            SDL_WarpMouseInWindow(window, TILE_SIZE, TILE_SIZE);
        }
        if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_RIGHT)){
            t->current_state = TITLE_SCREEN;
            SDL_WarpMouseInWindow(window, TILE_SIZE, TILE_SIZE);
        }
    }
    if(t->current_state == YOUWIN_SCREEN){
        if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_LEFT)){
            t->current_state = TITLE_SCREEN;
            SDL_WarpMouseInWindow(window, TILE_SIZE, TILE_SIZE);
        }
        if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_RIGHT)){
            t->current_state = TITLE_SCREEN;
            SDL_WarpMouseInWindow(window, TILE_SIZE, TILE_SIZE);
        }
    }
}

void menu_show(menu* t){
    switch(t->current_state){
        case LEVEL_SELECT_SCREEN:
        SDL_RenderCopy(renderer, t->screen_levelselect, NULL, NULL);
        break;
        case TITLE_SCREEN:
        SDL_RenderCopy(renderer, t->screen_title, NULL, NULL);
        break;
        case HTP_SCREEN:
        SDL_RenderCopy(renderer, t->screen_howtoplay, NULL, NULL);
        break;
        case GAMEOVER_SCREEN:
        SDL_RenderCopy(renderer, t->screen_gameover, NULL, NULL);
        break;
        case STORY_SCREEN:
        SDL_RenderCopy(renderer, t->screen_story, NULL, NULL);
        break;
        case YOUWIN_SCREEN:
        if(timeTakenToFinish > 999) timeTakenToFinish = 999;
        if(high_score[hs_currentlevel] > timeTakenToFinish){
            high_score[hs_currentlevel] = timeTakenToFinish;
        }
        SDL_RenderCopy(renderer, t->screen_win, NULL, NULL);
        { // render current time
            hud_drawnumber(7.4*TILE_SIZE, 5.10*TILE_SIZE, timeTakenToFinish);
            double decimalPart = ((int)(timeTakenToFinish*1000))%1000;
            { // draw point
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect dest = {8.65*TILE_SIZE, 5.6*TILE_SIZE, TILE_SIZE/16, TILE_SIZE/16};
                SDL_RenderFillRect(renderer, &dest);
            }
            hud_drawnumber(8.8*TILE_SIZE, 5.10*TILE_SIZE, decimalPart);
        }
        { // render best time
            hud_drawnumber(7.4*TILE_SIZE, 5.85*TILE_SIZE, high_score[hs_currentlevel]);
            double decimalPart = ((int)(high_score[hs_currentlevel]*1000))%1000;
            { // draw point
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect dest = {8.65*TILE_SIZE, 6.35*TILE_SIZE, TILE_SIZE/16, TILE_SIZE/16};
                SDL_RenderFillRect(renderer, &dest);
            }
            hud_drawnumber(8.8*TILE_SIZE, 5.85*TILE_SIZE, decimalPart);
        }
        break;
    }
}

void menu_destroy(menu* t){
    SDL_DestroyTexture(t->screen_levelselect);
    SDL_DestroyTexture(t->screen_title);
    SDL_DestroyTexture(t->screen_gameover);
    SDL_DestroyTexture(t->screen_howtoplay);
    SDL_DestroyTexture(t->screen_levelselect);
    SDL_DestroyTexture(t->screen_story);
    free(t);
}
