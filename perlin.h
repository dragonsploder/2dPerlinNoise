#ifdef IMPORT_PERLIN /* Header Guard  */
#include <stdlib.h>  /* Used for rand */
#include <math.h>    /* Used for PI   */

static int perlinSEED = 0; /* Used to change Perlin noise without calling reCalcPerlinHash */

/* Random array of numbers 0-255 */
static int perlinHash[256] = {208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                     185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                     9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                     70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                     203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                     164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                     228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                     232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                     193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                     101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                     135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                     114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};

/* Example function to create new gradient */
void reCalcPerlinHash(int seed){
    srand(seed);
    for(int i = 0; i < 256; i++){
        perlinHash[i] = rand()%256;
    }
}

/* Hash function to associate an x and y value with a number 0-255 */
int getPerlinHashValue(int x, int y){
    return perlinHash[(perlinHash[(y + perlinSEED) % 256] + x) % 256];
}

/* Classic liner interpolation function */
int lerp(float a, float b, float p){
    return (((1 - p) * a) + (p * b));
}

/* Cosine interpolation (think smooth lerp) using cos */
int cosInterpolation(float a, float b, float p){
   // float temp = p * p * (3 - (2*p));
    float temp = (0.5 * cos(M_PI * (p-1))) + 0.5;
    return lerp(a, b, temp);
}

/* Calculate one octave of perlin noise */
float singleOctavePerlin(float x, float y, int smooth){
    int gridX = (int) x;
    int gridY = (int) y;

    float extraX = x - gridX;
    float extraY = y - gridY;


    /* Perlin noise at its core is quite simple.
     * We start with a grid of random noise (see the perlinHash and getPerlinHashValue)
     * Because x and y are floats, we can split them into intergers and a value between 0-1 
     * (e.g. 4.563 would gives an interger of 4 and a leftover of 0.563). We use the intergers
     * to get values from our grid of random noise and then lerp between them.
     * 
     * For example, say x = 3.23 and y = 7.80. We look at the four grid points which surround 
     * the given point (3,7) (4,7) (3,8) (4,8). Each of these will give us a random value between 
     * 0-255. We then lerp between point the topLeft point (3,7) and the top right point (4,7) with
     * a lerp value of 0.23. We will do the same for the bottomLeft (3,8) and bottomRight (4,8) points.
     * Finally we will lerp between the two new values with a lerp value of 0.80. 
    */

    int topLeft = getPerlinHashValue(gridX, gridY);
    int topRight = getPerlinHashValue(gridX + 1, gridY);
    int bottomLeft = getPerlinHashValue(gridX, gridY + 1);
    int bottomRight = getPerlinHashValue(gridX + 1, gridY + 1);

    if (smooth){
        return cosInterpolation(cosInterpolation(topLeft, topRight, extraX), cosInterpolation(bottomLeft, bottomRight, extraX), extraY);
    } else {
        return lerp(lerp(topLeft, topRight, extraX), lerp(bottomLeft, bottomRight, extraX), extraY);
    }
}

/* Compleat function which can combind multiple octaves */
float perlin(float x, float y, float scale, int octave, int smooth){
    x = x * scale;
    y = y * scale;

    float amplitude = 1.0;
    float final = 0.0;
    float divide = 0.0;

    /*
     * To perform perlin noise at multipule octaves we simply shrink a larger
     * section and add it to our previous iteration.
    */ 
    for(int i = 0; i < octave; i++){
        final += singleOctavePerlin(x, y, smooth) * amplitude;
        divide += 256 * amplitude;
        amplitude /= 2;

        x = x * 2;
        y = y * 2;
    }
    return final/divide;
}

#undef IMPORT_PERLIN
#endif