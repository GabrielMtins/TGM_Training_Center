#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "mode7.h"

point lookupTableValues[MODE7_WIDTH][MODE7_HEIGHT];

void mode7_precalculateValues(){
    for(int j = -MODE7_HEIGHT; j < 0; j++){
        for(int i = -MODE7_WIDTH/2; i < MODE7_WIDTH/2; i++){
            int k = i+MODE7_WIDTH/2;
            int l = -j;
            double dist;
            dist = -0.75/((double)j/MODE7_HEIGHT);
            double x = 1*(double)i/(MODE7_HEIGHT)*dist;
            lookupTableValues[k][l].x = x;
            lookupTableValues[k][l].y = dist;
        }
    }
}


mode7_camera* mode7_createCamera(char* plane_surface_name){
    mode7_camera* camera = malloc(sizeof(mode7_camera));
    camera->x = 5;
    camera->y = 5;
    camera->direction = PI/32;
    camera->scale = 32;
    SDL_Surface* plane_surface = IMG_Load(plane_surface_name);
    camera->draw_surface = SDL_CreateRGBSurface(0, MODE7_WIDTH, MODE7_HEIGHT, 32, 0, 0, 0, 0);
    camera->width = plane_surface->w;
    camera->height = plane_surface->h;
    camera->plane_buffer = malloc(3*camera->width*camera->height);
    for(int i = 0; i < camera->width; i++){
        for(int j = 0; j < camera->height; j++){
            uint32_t* pixels = plane_surface->pixels;
            uint8_t r, g, b, a;
            SDL_GetRGBA(pixels[i+camera->width*j],
                plane_surface->format,
                &r,
                &g,
                &b,
                &a
            );
            camera->plane_buffer[3*(i+j*camera->width)+0] = r;
            camera->plane_buffer[3*(i+j*camera->width)+1] = g;
            camera->plane_buffer[3*(i+j*camera->width)+2] = b;
        }
    }
    SDL_FreeSurface(plane_surface);
    return camera;
}

SDL_Color mode7_getPixelColor(mode7_camera* camera, int pixel_x, int pixel_y, double cos_value, double sin_value){
    double x_value = lookupTableValues[pixel_x][pixel_y].x;
    double y_value = lookupTableValues[pixel_x][pixel_y].y;
    double x_cords = x_value*cos_value-y_value*sin_value;
    double y_cords = x_value*sin_value+y_value*cos_value;
    x_cords+=camera->x;
    y_cords+=camera->y;
    if(x_cords <= 0 || y_cords <= 0 || x_cords*camera->scale >= camera->width || y_cords*camera->scale >= camera->height){
        int valueOfTexture_x = camera->scale*x_cords;
        int valueOfTexture_y = camera->scale*y_cords;
        if(valueOfTexture_x < 0) valueOfTexture_x = -valueOfTexture_x;
        if(valueOfTexture_y < 0) valueOfTexture_y = -valueOfTexture_y;
        valueOfTexture_x = valueOfTexture_x-((valueOfTexture_x>>4)<<4); // fast module
        valueOfTexture_y = valueOfTexture_y-((valueOfTexture_y>>4)<<4);
        SDL_Color color = {
            camera->plane_buffer[3*(valueOfTexture_x+valueOfTexture_y*camera->width)+0],
            camera->plane_buffer[3*(valueOfTexture_x+valueOfTexture_y*camera->width)+1],
            camera->plane_buffer[3*(valueOfTexture_x+valueOfTexture_y*camera->width)+2]
        };
        return color;
    }
    else{
        int valueOfTexture_x = camera->scale*x_cords;
        int valueOfTexture_y = camera->scale*y_cords;
        if(valueOfTexture_x > camera->width) valueOfTexture_x %=camera->width;
        if(valueOfTexture_y > camera->height) valueOfTexture_y %=camera->height;
        SDL_Color color = {
            camera->plane_buffer[3*(valueOfTexture_x+valueOfTexture_y*camera->width)+0],
            camera->plane_buffer[3*(valueOfTexture_x+valueOfTexture_y*camera->width)+1],
            camera->plane_buffer[3*(valueOfTexture_x+valueOfTexture_y*camera->width)+2]
        };
        return color;
    }
}

void mode7_renderCamera(mode7_camera* camera){
    double cos_value = cos(-0.5*PI+camera->direction);
    double sin_value = sin(-0.5*PI+camera->direction);
    for(int j = 1; j < MODE7_HEIGHT; j++){
        for(int i = 0; i < MODE7_WIDTH; i++){
            SDL_Color color = mode7_getPixelColor(camera, i, j, cos_value, sin_value);
            uint32_t* pixels = camera->draw_surface->pixels;
            pixels[i+j*MODE7_WIDTH] = SDL_MapRGB(camera->draw_surface->format, color.r, color.g, color.b);
        }
    }
    SDL_Texture* draw_texture = SDL_CreateTextureFromSurface(renderer, camera->draw_surface);
    SDL_Rect dst = {0, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT/2};
    SDL_RenderCopy(renderer, draw_texture, NULL, &dst);
    SDL_DestroyTexture(draw_texture);
}

void mode7_destroy(mode7_camera* camera){
    if(camera == NULL) return;
    SDL_FreeSurface(camera->draw_surface);
    free(camera->plane_buffer);
    free(camera);
}
