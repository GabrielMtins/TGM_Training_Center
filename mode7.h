#ifndef MODE7_H
#define MODE7_H

#include "global.h"

#define MODE7_WIDTH 854
#define MODE7_HEIGHT 240

// rendering in 480p (854 x 480) for better fps
// if you are reading the source code (for some reason??) you
// can change the width and height to render in 720p or higher

#define NUM_OF_COS 3200
#define PI 3.141592

typedef struct{
    double x, y;
} point;

extern point lookupTableValues[MODE7_WIDTH][MODE7_HEIGHT];

typedef struct{
    double x, y; // coordinates position
    double direction;
    double scale;
    int width, height;
    uint8_t* plane_buffer;
    SDL_Surface* draw_surface;
} mode7_camera;

void mode7_precalculateValues();

mode7_camera* mode7_createCamera(char* plane_surface_name);

SDL_Color mode7_getPixelColor(mode7_camera* camera, int pixel_x, int pixel_y, double cos_value, double sin_value);

void mode7_renderCamera(mode7_camera* camera);

void mode7_destroy(mode7_camera* camera);

#endif
