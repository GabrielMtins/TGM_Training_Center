#include "load.h"

#include "player.h"

#include <stdio.h>

void load_fromMap(char* filename){
    if(mainPlayer != NULL){
        player_destroy(mainPlayer);
    }
    FILE* file = fopen(filename, "r");
    int x_player_pos, y_player_pos;
    fscanf(file, "%i %i", &x_player_pos, &y_player_pos);
    char level_name[256] = "";
    fscanf(file, "%s", level_name);
    char collision_name[256] = "";
    fscanf(file, "%s", collision_name);
    char sky_name[256] = "";
    fscanf(file, "%s", sky_name);
    mainPlayer = player_create(level_name);
    player_loadSky(mainPlayer, sky_name);
    player_loadCollisionMap(mainPlayer, collision_name);
    mainPlayer->x = x_player_pos;
    mainPlayer->y = y_player_pos;
    fclose(file);
}
