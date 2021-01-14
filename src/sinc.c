// This code creates the image to the right >>
//
// (c) 2020 Mike Field <hamster@snap.net.nz>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define SIDE 4660
#define TRACES 40
#define DIV    (TRACES+20)
#define GAP    (SIDE/DIV)
#define SEGMENTS 40

unsigned char p[SIDE][SIDE];

double line[SEGMENTS+1];

int main(int argc, char *argv[])
{
   int trace, x, y;

   if(argc != 2) {
      printf("Please supply output file name\n");
      return -1;
   }
 
   printf("Creating file %s\n", argv[1]);
   FILE *file = fopen(argv[1],"w");
   if(file == NULL) {
      printf("Unable to open output image file\n");
      return -1;
   }

   fprintf(file, "P6\n%i %i\n255\n", SIDE, SIDE);

   for(trace = 0; trace <= TRACES; trace++) {
      int baseline = (trace+(DIV-TRACES)/2)*GAP;
      int points[SIDE/2];

      for(x = 0; x < SIDE/2; x++) {
         double dx = (x-SIDE/4.0)/(SIDE/2.0)*25.0;
         double dy = (trace-TRACES/2.0)/(TRACES/2.0)*20.0;
         double d  = sqrt(dx*dx+dy*dy+0.001);
         points[x]  = baseline - sin(d)/d*SIDE/10.0;
      }

      for(x = 0; x < SIDE/2; x++) {
         for(y=points[x]+3; y < SIDE; y++)
            for(int j = -2; j <=2; j++)
               p[y][x+SIDE/4+j] = 0;
      }

      for(x = 0; x < SIDE/2; x++) {
         for(int i = -2; i <=2; i++)
            for(int j = -2; j <=2; j++)
              p[points[x]+0+i][x+SIDE/4+0+j] = 255;
      }
   }

   for(y = 0; y < SIDE; y++ ) {
      for(x = 0; x < SIDE; x++) {
         putc(p[y][x],      file);
         putc(p[y][x],      file);
         putc(p[y][x],      file);
      }
   }
   fclose(file);
   return 0;
}
