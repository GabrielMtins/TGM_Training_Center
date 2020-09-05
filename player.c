#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

double timeTakenToFinish;
double high_score[4];
int hs_currentlevel;

#define HEALING_AREA 2
#define ENDING_AREA 3
#define SLOW_SPEED 4

void player_loadHighScores(char* filename){
    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        high_score[0] = high_score[1] = high_score[2] = high_score[3] = 999;
        return;
    }
    fread(high_score, sizeof(double), 4*sizeof(double), file);
    fclose(file);
}

void player_saveHighScores(char* filename){
    FILE* file = fopen(filename, "wb");
    fwrite(high_score, sizeof(double), 4*sizeof(double), file);
    fclose(file);
}

player* player_create(char* map_filename){
    timeTakenToFinish = 0;
    player* p = malloc(sizeof(player));
    p->x = p->y = p->accx = p->accy = p->vx = p->vy = 0;
    p->x = 1; p->y = 2;
    p->direction = 0;
    p->hasFinished = 0;
    p->fuel = 70;
    p->collision_map = NULL;
    p->player_camera = mode7_createCamera(map_filename);
    p->listOfSprites = NULL;
    p->numOfSprites = 0;
    return p;
}

void player_destroyCollisionMap(player* p){
    if(p->collision_map == NULL) return;
    free(p->collision_map);
    p->collision_map = NULL;
}

void player_loadCollisionMap(player* p, char* collision_filename){
    SDL_Surface* collision_surface = IMG_Load(collision_filename);
    p->width_collision = collision_surface->w;
    p->height_collision = collision_surface->h;
    p->collision_map = malloc(p->width_collision*p->height_collision);
    for(int i = 0; i < p->width_collision; i++){
        for(int j = 0; j < p->height_collision; j++){
            uint32_t* pixels = collision_surface->pixels;
            uint8_t r, g, b, a;
            SDL_GetRGBA(pixels[i+p->width_collision*j],
                collision_surface->format,
                &r,
                &g,
                &b,
                &a
            );
            if(r == 255 && g == 255 && b == 255){
                p->collision_map[i+j*p->width_collision] = 1;
            }
            else if(g == 255 && r == 0 && b == 0){
                p->collision_map[i+j*p->width_collision] = HEALING_AREA;
            }
            else if(g ==0 && r == 0 && b == 255){
                p->collision_map[i+j*p->width_collision] = ENDING_AREA;
            }
            else if(r == 255 && g == 255 && b == 0){
                p->collision_map[i+j*p->width_collision] = SLOW_SPEED;
            }
            else if(r == 255 && g == 0){
                p->numOfSprites++;
                p->listOfSprites = realloc(p->listOfSprites, p->numOfSprites*sizeof(sprite));
                p->listOfSprites[p->numOfSprites-1].x = (double)i/p->player_camera->scale;
                p->listOfSprites[p->numOfSprites-1].y = (double)j/p->player_camera->scale;
                p->listOfSprites[p->numOfSprites-1].spr_profile = &allSpriteProfiles[b];
            }
            else p->collision_map[i+j*p->width_collision] = 0;
        }
    }
    SDL_FreeSurface(collision_surface);
}

void player_loadSky(player* p, char* sky_filename){
    SDL_Surface* surface = IMG_Load(sky_filename);
    p->sky_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void player_input(player* p){
    { // input
        const unsigned char* currentKeyState = SDL_GetKeyboardState(NULL);
        if(currentKeyState[SDL_SCANCODE_W]){
            p->accx+=cos(p->direction)*PLAYER_ACCELERATION;
            p->accy+=sin(p->direction)*PLAYER_ACCELERATION;
        }
        else{
            if(currentKeyState[SDL_SCANCODE_S]){
                p->accx+=-cos(p->direction)*PLAYER_ACCELERATION/2;
                p->accy+=-sin(p->direction)*PLAYER_ACCELERATION/2;
            }
            else{
                p->accx+=-cos(p->direction)*PLAYER_ACCELERATION/4;
                p->accy+=-sin(p->direction)*PLAYER_ACCELERATION/4;
            }
        }

        if(currentKeyState[SDL_SCANCODE_A]){
            if(p->vx*p->vx+p->vy*p->vy > (double)PLAYER_VELOCITY*PLAYER_VELOCITY/100){
                p->accx += 1.5*PLAYER_ACCELERATION*cos(p->direction+PI/2);
                p->accy += 1.5*PLAYER_ACCELERATION*sin(p->direction+PI/2);
            }
            else{
                p->accx += 0.075*PLAYER_ACCELERATION*cos(p->direction+PI/2);
                p->accy += 0.075*PLAYER_ACCELERATION*sin(p->direction+PI/2);
            }
        }
        if(currentKeyState[SDL_SCANCODE_D]){
            if(p->vx*p->vx+p->vy*p->vy > (double)PLAYER_VELOCITY*PLAYER_VELOCITY/100){
                p->accx += 1.5*PLAYER_ACCELERATION*cos(p->direction-PI/2);
                p->accy += 1.5*PLAYER_ACCELERATION*sin(p->direction-PI/2);
            }
            else{
                p->accx += 0.075*PLAYER_ACCELERATION*cos(p->direction-PI/2);
                p->accy += 0.075*PLAYER_ACCELERATION*sin(p->direction-PI/2);
            }
        }
        if(currentKeyState[SDL_SCANCODE_ESCAPE]){
            p->fuel = 0;
        }
        if(p->direction < 0) p->direction+=2*PI;
        if(p->direction >= 2*PI) p->direction-=2*PI;
    }
}

void player_update(player* p){
    player_input(p);
    if(p->fuel >= 100) p->fuel = 100;
    if(p->fuel <= 0) return;
    timeTakenToFinish+=deltaTime;
    p->vx+=p->accx*deltaTime;
    p->vy+=p->accy*deltaTime;
    p->direction = atan2(p->vy, p->vx);
    p->accx = 0;
    p->accy = 0;
    if(p->vx != 0 || p->vy != 0) p->fuel-=deltaTime*4;
    if(p->vx*p->vx + p->vy*p->vy > PLAYER_VELOCITY*PLAYER_VELOCITY){
        p->vx = PLAYER_VELOCITY*cos(p->direction);
        p->vy = PLAYER_VELOCITY*sin(p->direction);
    }
    if(p->vx*p->vx + p->vy*p->vy < (double)PLAYER_VELOCITY*PLAYER_VELOCITY/10000){
        p->vx = PLAYER_VELOCITY*cos(p->direction)/100;
        p->vy = PLAYER_VELOCITY*sin(p->direction)/100;
    }
    p->x+=p->vx*deltaTime;
    {
        int collision = 0;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                int x = p->x*p->player_camera->scale;
                int y = p->y*p->player_camera->scale;
                if(p->collision_map[x+i+(y+j)*p->width_collision] == 1 || x < 0 || y < 0){
                    collision = 1;
                }
            }
        }
        if(collision){
            p->x-=p->vx*deltaTime;
        }
    }
    p->y+=p->vy*deltaTime;
    {
        int collision = 0;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                int x = p->x*p->player_camera->scale;
                int y = p->y*p->player_camera->scale;
                if(p->collision_map[x+i+(y+j)*p->width_collision] == 1 || x < 0 || y < 0){
                    collision = 1;
                }
            }
        }
        if(collision){
            p->y-=p->vy*deltaTime;
        }
    }
    { // check if we are in a healing area
        int x = p->x*p->player_camera->scale;
        int y = p->y*p->player_camera->scale;
        if(p->collision_map[x+(y)*p->width_collision] == HEALING_AREA){
            p->fuel+=40*deltaTime;
        }
    }
    { // check if we are in a slow speed area
        int x = p->x*p->player_camera->scale;
        int y = p->y*p->player_camera->scale;
        if(p->collision_map[x+(y)*p->width_collision] == SLOW_SPEED){
            p->vx = PLAYER_VELOCITY/4*cos(p->direction);
            p->vy = PLAYER_VELOCITY/4*sin(p->direction);
        }
    }
    { // check if we finished
        int x = p->x*p->player_camera->scale;
        int y = p->y*p->player_camera->scale;
        if(x >= 0 || y >= 0){
            if(p->collision_map[x+(y)*p->width_collision] == ENDING_AREA){
                p->hasFinished = 1;
            }
        }
    }
    if(p->direction < 0) p->direction+=2*PI;
    if(p->direction >= 2*PI) p->direction-=2*PI;
}

void player_renderSprites(player* p){
    if(p->listOfSprites == NULL) return;
    sprite* alteredListOfSprites = malloc(sizeof(sprite)*p->numOfSprites);
    memcpy(alteredListOfSprites, p->listOfSprites, sizeof(sprite)*p->numOfSprites);
    double cos_direction = cos(-p->direction+PI/2);
    double sin_direction = sin(-p->direction+PI/2);
    for(int i = 0; i < p->numOfSprites; i++){
        alteredListOfSprites[i].x -= p->x;
        alteredListOfSprites[i].y -= p->y;
        double newX = cos_direction*alteredListOfSprites[i].x-sin_direction*alteredListOfSprites[i].y;
        double newY = sin_direction*alteredListOfSprites[i].x+cos_direction*alteredListOfSprites[i].y;
        alteredListOfSprites[i].x = newX;
        alteredListOfSprites[i].y = newY;
    }
    // sort by distance
    for(int i = 0; i < p->numOfSprites; i++){
        for(int j = 0; j < p->numOfSprites-1-i; j++){
            if(alteredListOfSprites[j].y < alteredListOfSprites[j+1].y){
                sprite swap = alteredListOfSprites[j];
                alteredListOfSprites[j] = alteredListOfSprites[j+1];
                alteredListOfSprites[j+1] = swap;
            }
        }
    }
    for(int i = 0; i < p->numOfSprites; i++){
        if(alteredListOfSprites[i].y > 0){
            sprite_render(alteredListOfSprites+i);
        }
    }
    free(alteredListOfSprites);
}

void player_render(player* p){
    p->player_camera->x = p->x;
    p->player_camera->y = p->y;
    p->player_camera->direction = p->direction;
    mode7_renderCamera(p->player_camera);
    { // render sky;
        double sky_angle = 2*PI-p->direction;
        SDL_Rect src = {sky_angle/(PI)*854, 0, 854, 240};
        SDL_Rect dest = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/2+2};
        SDL_RenderCopy(renderer, p->sky_texture, &src, &dest);
    }
    player_renderSprites(p);
}

void player_destroy(player* p){
    if(p == NULL) return;
    if(p->sky_texture != NULL) SDL_DestroyTexture(p->sky_texture);
    if(p->listOfSprites != NULL) free(p->listOfSprites);
    player_destroyCollisionMap(p);
    mode7_destroy(p->player_camera);
    free(p);
    p = NULL;
}
