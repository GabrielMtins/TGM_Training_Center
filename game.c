#include "global.h"
#include "mode7.h"
#include "player.h"
#include "hud.h"
#include "title.h"
#include "load.h"
#include "sprite.h"

#include <stdio.h>

menu* mainMenu;
player* mainPlayer = NULL;
sprite_profile* allSpriteProfiles = NULL;
int quit = 0;

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_DisplayMode dm;
    if(SDL_GetCurrentDisplayMode(0, &dm)<0){
        return;
    }
    WINDOW_WIDTH = dm.w;
    WINDOW_HEIGHT = dm.h;
    TILE_SIZE = WINDOW_HEIGHT/9;
    window = SDL_CreateWindow("The Great Machine Training Center", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    mainMenu = menu_create();
    mainPlayer = NULL;
    sprite_loadAllProfiles("textures/sprites/sprite_list.txt");
    mode7_precalculateValues();
    hud_load();
    player_loadHighScores("levels/hs.bin");
}

void game_update(){
    if(mainMenu->current_state == NO_SCREEN) player_update(mainPlayer);
    else menu_update(mainMenu);
    if(mainPlayer != NULL && mainPlayer->fuel <= 0){
        player_destroy(mainPlayer);
        mainPlayer = NULL;
        mainMenu->current_state = GAMEOVER_SCREEN;
    }
    if(mainPlayer != NULL && mainPlayer->hasFinished){
        player_destroy(mainPlayer);
        mainPlayer = NULL;
        mainMenu->current_state = YOUWIN_SCREEN;
    }
}

void game_render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if(mainMenu->current_state == NO_SCREEN){
        SDL_RenderClear(renderer);
        player_render(mainPlayer);
        hud_drawhud(mainPlayer);
    }
    else menu_show(mainMenu);
    SDL_RenderPresent(renderer);
}

void game_loop(){
    SDL_Event e;
    Uint32 firstTime;
    while(!quit){
        firstTime = SDL_GetTicks();
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) quit = 1;
        }
        game_update();
        game_render();
        deltaTime = (double)(SDL_GetTicks()-firstTime)/1000;
        if(deltaTime > 0.25) deltaTime = 0;
    }
}

void game_close(){
    player_saveHighScores("levels/hs.bin");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    menu_destroy(mainMenu);
    if(mainPlayer != NULL) player_destroy(mainPlayer);
    hud_destroy();
    sprite_destroyProfiles();
    IMG_Quit();
    SDL_Quit();
    return;
}

void game(){
    init();
    game_loop();
    game_close();
}
