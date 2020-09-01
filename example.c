#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define IMPORT_PERLIN
#include "perlin.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <stdbool.h>
#include <stdint.h>

#define WIDTH 700
#define HIGHT 300
int main(){
    srand(time(NULL));
    uint8_t pixels[WIDTH * HIGHT * 3];

    int index = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    int value = 0;

    perlinSEED = (int) time(NULL);
    

    for(int i = 0; i < HIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            value = round(perlin(i, j, 0.01, 2, true) * 255);
            //value = round(value / 20) * 20; /* Try uncommenting this */
            pixels[index++] = value;
            pixels[index++] = value;
            pixels[index++] = value;

        }
    }
 

    stbi_write_png("perlin.png", WIDTH, HIGHT, 3, &pixels, WIDTH * 3);

    return 0;
}    