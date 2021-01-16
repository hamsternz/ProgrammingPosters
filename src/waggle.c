// This code creates the image to the right >>
//
// (c) 2021 Mike Field <hamster@snap.net.nz>
//          Inspired by Zodian on Discord
//
#include <stdio.h>
#include <math.h>

#define WIDTH  4660
#define HEIGHT 4660

// Frequencies
const float f1 = 2.01;
const float f2 = 3.00;
const float f3 = 3.01;
const float f4 = 1.00;
// Damp Force
const float d1 = 0.0001;
const float d2 = 0.0001;
const float d3 = 0.001;
const float d4 = 0.0001;
// Phases
const float p1 = 0;
const float p2 = 4 * M_PI / 16;
const float p3 = 0;
const float p4 = 0;

const float amp = HEIGHT/5.0;

unsigned char pixels[HEIGHT][WIDTH];

int main(int argc, char *argv[]){
   if(argc != 2) {
      fprintf(stderr,"Please supply image name\n");
      return 0;
   }


   int x, y;
   float t = 0;
   float dt = M_PI/(WIDTH+HEIGHT);
   int iterations = 100*(WIDTH+HEIGHT);

   for ( int i = 0; i < iterations; i++ ){
      x = amp * sin(f1*t+p1) * pow(M_E,(-t*d1)) +
          amp * sin(f2*t+p2) * pow(M_E,(-t*d2));
      y = amp * sin(f3*t+p3) * pow(M_E,(-t*d3)) +
          amp * sin(f4*t+p4) * pow(M_E,(-t*d4));

      x = ( int ) x + ( WIDTH / 2 );
      y = ( int ) y + ( HEIGHT / 2 );
      if(x > 1 && y > 1 && y < HEIGHT-1 && x < WIDTH-1) {
         pixels[y+1][x-1] = 1+i*254/iterations;
         pixels[y+0][x-1] = 1+i*254/iterations;
         pixels[y-1][x-1] = 1+i*254/iterations;
         pixels[y+1][x+0] = 1+i*254/iterations;
         pixels[y+0][x+0] = 1+i*254/iterations;
         pixels[y-1][x+0] = 1+i*254/iterations;
         pixels[y+1][x+1] = 1+i*254/iterations;
         pixels[y+0][x+1] = 1+i*254/iterations;
         pixels[y-1][x+1] = 1+i*254/iterations;
      }
      t += dt;
   }

   FILE *f = fopen(argv[1], "wb");
   if(f != NULL) {
      fprintf(f,"P6\n%i %i\n255\n",HEIGHT,WIDTH);
      for(y = 0; y < HEIGHT; y++) {
         for(x = 0; x < WIDTH; x++) {
            if(pixels[y][x]>0) {
               putc(128+pixels[y][x]/2,f);
               putc(128-pixels[y][x]/2,f);
               putc(255-pixels[y][x]/2,f);
            } else {
               putc(0,f);
               putc(0,f);
               putc(0,f);
            }
         }
      }
      fclose(f);
   }
   return 0;
}



