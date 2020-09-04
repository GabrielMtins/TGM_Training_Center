#include "sprite.h"
#include "mode7.h"

#include <stdio.h>
#include <stdlib.h>

int numOfProfiles = 0;

void sprite_loadAllProfiles(char* filename){
    FILE* file = fopen(filename, "r");
    int numOfSpriteProfiles;
    fscanf(file, "%i", &numOfSpriteProfiles);
    allSpriteProfiles = malloc(sizeof(sprite_profile)*numOfSpriteProfiles);
    for(int i = 0; i < numOfSpriteProfiles; i++){
        char texture_name[256] = "";
        fscanf(file, "%s %lf %lf", texture_name, &allSpriteProfiles[i].width, &allSpriteProfiles[i].height);
        SDL_Surface* surface = IMG_Load(texture_name);
        allSpriteProfiles[i].spr_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    numOfProfiles = numOfSpriteProfiles;
    fclose(file);
}

void sprite_render(sprite* s){
    double x_image = s->x/s->y;
    double y_image = -1/s->y;
    x_image *= MODE7_HEIGHT;
    y_image *= MODE7_HEIGHT;
    x_image+=MODE7_WIDTH>>1;
    y_image+=MODE7_HEIGHT>>1;
    double height_image = s->spr_profile->height*MODE7_HEIGHT/s->y;
    double width_image = height_image*s->spr_profile->width;
    double x_rect = x_image*WINDOW_WIDTH/MODE7_WIDTH;
    double y_rect = y_image*WINDOW_HEIGHT/MODE7_HEIGHT;
    double w_rect = width_image*WINDOW_WIDTH/MODE7_WIDTH;
    double h_rect = height_image*WINDOW_HEIGHT/MODE7_HEIGHT;
    SDL_Rect dest = {x_rect, y_rect, w_rect, h_rect};
    SDL_RenderCopy(renderer, s->spr_profile->spr_texture, NULL, &dest);
}

void sprite_destroyProfiles(){
    for(int i = 0; i < numOfProfiles; i++){
        SDL_DestroyTexture(allSpriteProfiles[i].spr_texture);
    }
    free(allSpriteProfiles);
}
