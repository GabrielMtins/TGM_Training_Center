#include "hud.h"
#include <math.h>

SDL_Texture* hud_texture = NULL;
SDL_Texture* numbers_texture = NULL;
SDL_Texture* velocimeter_texture = NULL;

void hud_load(){
    { // load texture
        SDL_Surface* surface = IMG_Load("textures/hud/hud.png");
        hud_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    { // load texture
        SDL_Surface* surface = SDL_LoadBMP("textures/hud/numbers.bmp");
        numbers_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    { // load texture
        SDL_Surface* surface = IMG_Load("textures/hud/velocimeter.png");
        velocimeter_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void hud_drawnumber(int x, int y, double number){
    int number_int = (int)number;
    int first_digit = (number_int%100)%10;
    int second_digit = ((number_int%100)-first_digit)/10;
    int third_digit = (number_int-second_digit*10-first_digit)/100;
    {
        SDL_Rect dest = {x, y, WINDOW_WIDTH/42, WINDOW_HEIGHT/16};
        SDL_Rect src = {third_digit*10, 0, 10, 15};
        SDL_RenderCopy(renderer, numbers_texture, &src, &dest);
    }
    {
        SDL_Rect dest = {x+WINDOW_WIDTH/38, y, WINDOW_WIDTH/42, WINDOW_HEIGHT/16};
        SDL_Rect src = {second_digit*10, 0, 10, 15};
        SDL_RenderCopy(renderer, numbers_texture, &src, &dest);
    }
    {
        SDL_Rect dest = {x+WINDOW_WIDTH/19, y, WINDOW_WIDTH/42, WINDOW_HEIGHT/16};
        SDL_Rect src = {first_digit*10, 0, 10, 15};
        SDL_RenderCopy(renderer, numbers_texture, &src, &dest);
    }
}

void hud_drawhud(player* p){
    {
        SDL_Rect dest = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, hud_texture, NULL, &dest);
    }
    { // render fuel bar;
        SDL_Rect dest = {2.1*TILE_SIZE, 2.17*TILE_SIZE, 3.35*TILE_SIZE*p->fuel/100, 0.471*TILE_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &dest);
    }
    { // render the velocimeter
        double speed = sqrt(p->vx*p->vx+p->vy*p->vy)/PLAYER_VELOCITY;
        if(speed > 0.95) speed = 1-(double)1/17;
        int x_rect = speed*17;
        SDL_Rect src = {64*x_rect, 0, 64, 32};
        SDL_Rect dest = {2.1*TILE_SIZE, 6.4*TILE_SIZE, TILE_SIZE*2, TILE_SIZE};
        SDL_RenderCopy(renderer, velocimeter_texture, &src, &dest);
    }
}

void hud_destroy(){
    SDL_DestroyTexture(hud_texture);
    SDL_DestroyTexture(numbers_texture);
    SDL_DestroyTexture(velocimeter_texture);
}
