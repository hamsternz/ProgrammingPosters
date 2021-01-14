// This code creates the image to the right
// 
// (c) 2020 Mike Field <hamster@snap.net.nz>
//
// Blue Marble data from NASA:
// https://visibleearth.nasa.gov/collection/1484/blue-marble

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define TEXTURE_WIDTH  2048
#define TEXTURE_HEIGHT 1024
static unsigned char marble[TEXTURE_HEIGHT][TEXTURE_WIDTH][3];

int main(int argc, char *argv[])
{
   int width  = 4660;
   int height = 4660;
   FILE *file;

   if(argc != 2) {
      printf("You need to supply the output file name.\n");
      return -1;
   }
   printf("Creating file %s\n", argv[1]);

   file = fopen("texture/bluemarble.ppm", "r");
   if(file == NULL) {
      printf("Unable to open texture file\n");
      return -1;
   }
   fseek(file,17,SEEK_SET);
   if(fread(marble,sizeof(marble),1,file) != 1) {
      printf("Unable to read texture file\n");
      return -1;
   }
   fclose(file);
 
   file = fopen(argv[1],"w");
   if(file == NULL) {
      printf("Unable to open output file\n");
      return -1;
   }

   fprintf(file, "P6\n%i %i\n255\n",width, height);

   int i;
   for(i = 0; i < width; i++ ) {
      int j;
      for(j = 0; j < height; j++) {
         int y = i - width/2;
         int x = j - width/2;

         if(x*x+ y*y < height/4*height/4) {
            double dx = x/(height/4.0);
            double dy = y/(height/4.0);
            double dz = sqrt(1.0 - dx*dx - dy*dy);

            double longitude  = -atan2(dz,dx)/(M_PI*2)+0.5;
            double latitude   = atan2(dy,sqrt(dx*dx+dz*dz))/(M_PI)+0.5;

            double light = ( dx*0.806 + dy*0.000 + dz*0.500);
            if(light < 0) light = 0;

            if(latitude < 0) latitude = 0.0;
            if(longitude < 0) longitude = 0.0;
            if(latitude >= 1.0) latitude = 0.0;
            if(longitude >= 1.0) longitude = 0.0;

            int r = marble[(int)(latitude*1024)][(int)(longitude*2048)][0];
            int g = marble[(int)(latitude*1024)][(int)(longitude*2048)][1];
            int b = marble[(int)(latitude*1024)][(int)(longitude*2048)][2];

            r *= light;
            g *= light;
            b *= light;

            putc(r, file);  // Red
            putc(g, file);  // Green
            putc(b, file);  // Blue
         } else {
            if(rand() < RAND_MAX/(width/100)) {
               int star = (float)rand()*rand()*rand();
               star /= 255.0*RAND_MAX/RAND_MAX/RAND_MAX;
               putc(star, file);  // Red
               putc(star, file);  // Green
               putc(star, file);  // Blue
            } else  {
               putc(2, file);  // Red
               putc(2, file);  // Green
               putc(2, file);  // Blue
            }
         }
      }
   }
   fclose(file);
   return 0;
}
